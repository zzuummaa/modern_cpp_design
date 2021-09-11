#include <fstream>
#include <string>
#include <utility>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <iostream>
#include <cstring>

struct Message {
    int id;
    int len;
    unsigned char data[64];
};

template<typename IMessageType>
class IDevice {
public:
    virtual bool open() = 0;
    virtual bool write(const IMessageType&) = 0;
    virtual bool read(IMessageType& message) = 0;
    virtual bool close() = 0;
};

enum class ErrorType {
    None = 0,
    UnknownDevice = 'd',
    WriteError = 'w',
    ReadError = 'r',
    ConvertError = 'c',
    UnknownError = 'u'
};

template<
    typename HandleAccessor,
    typename ErrorHandler,
    typename MessageConverter
>
class Device : public IDevice<typename MessageConverter::FirstType> {
    typedef typename HandleAccessor::HandleType HandleType;
    typedef typename MessageConverter::FirstType IMessageType;
    typedef typename MessageConverter::SecondType NativeMessageType;
public:
    explicit Device(const std::string& path) : path(path), handle() {}

    bool open() override {
        auto open_error = HandleAccessor::open(handle, path);
        return ErrorHandler::process(open_error);
    }

    bool write(const IMessageType& message) override {
        NativeMessageType native_message;
        auto convert_error = MessageConverter::convert(message, native_message);
        if (!ErrorHandler::process(convert_error)) return false;

        auto write_error = HandleAccessor::write(handle, native_message);
        return ErrorHandler::process(write_error);
    }

    bool read(IMessageType& message) override {
        NativeMessageType native_message;
        auto read_error = HandleAccessor::read(handle, native_message);
        if (!ErrorHandler::process(read_error)) return false;

        auto convert_error = MessageConverter::convert(std::as_const(native_message), message);
        return ErrorHandler::process(convert_error);
    }

    bool close() override {
        auto close_error = HandleAccessor::close(handle);
        return ErrorHandler::process(close_error);
    }

private:
    std::string path;
    HandleType handle;
};

template<std::ios_base::openmode OpenMode>
class LineFileStreamHandleAccessor {
public:
    typedef std::fstream HandleType;

    static ErrorType open(HandleType& handle, const std::string& path) {
        handle.open(path, OpenMode);
        return handle.is_open() ? ErrorType::None : ErrorType::UnknownDevice;
    }

    static ErrorType read(HandleType& handle, std::string& message) {
        std::getline(handle, message);
        return handle.good() ? ErrorType::None : ErrorType::ReadError;
    }

    static ErrorType write(HandleType& handle, std::string& message) {
        handle << message << std::endl;
        return handle.good() ? ErrorType::None : ErrorType::WriteError;
    }

    static ErrorType close(HandleType& handle) {
        handle.close();
        return handle.fail() ? ErrorType::UnknownError : ErrorType::None;
    }
};

static int hex_to_dec(char c) {
    return (c < '0')  ? -1 :
           (c <= '9') ? (c - '0') :
           (c < 'A')  ? -1 :
           (c <= 'F') ? (c - 'A' + 10) :
           (c < 'a')  ? -1 :
           (c <= 'f') ? (c - 'a' + 10) : -1;
}

class MessageStringConverter {
public:
    typedef Message FirstType;
    typedef std::string SecondType;

    static ErrorType convert(const FirstType& first, SecondType& second) {
        std::stringstream ss;
        ss << first.id << " " << first.len << " ";
        ss << std::hex;

        for(int i = 0; i < std::min(std::size(first.data), static_cast<std::size_t>(first.len)); ++i ) {
            ss << std::setw(2) << std::setfill('0') << (int)first.data[i];
        }

        if (ss.fail()) return ErrorType::ConvertError;

        second = std::move(ss.str());
        return ErrorType::None;
    }

    static ErrorType convert(const SecondType& second, FirstType& first) {
        std::istringstream ss(second);
        ss >> first.id >> first.len;

        if (first.len > std::size(first.data)) return ErrorType::ConvertError;
        std::memset(first.data, 0, first.len * sizeof(first.data[0]));

        char c;

        int i = 0;
        while (ss >> c) {
            if (i / 2 >= std::size(first.data)) return ErrorType::ConvertError;

            int d = hex_to_dec(c);
            if (d < 0) return ErrorType::ConvertError;
            first.data[i / 2] |= i % 2 ? d : d << 4;
            i++;
        }
        if (i % 2 != 0) return ErrorType::ConvertError;

        bool is_ok = (ss.rdstate() & std::ios_base::eofbit) && !(ss.rdstate() & std::ios_base::failbit);
        return is_ok ? ErrorType::ConvertError : ErrorType::None;
    }
};

std::ostream& operator<<(std::ostream& os, const Message& message) {
    MessageStringConverter::SecondType native_message;
    MessageStringConverter::convert(message, native_message);
    os << native_message;
    return os;
}

class SimpleErrorHandler {
public:
    template<typename ErrorType>
    static bool process(ErrorType error) {
        return error == (ErrorType)0;
    }
};

class ThrowErrorHandler {
public:
    template<typename ErrorType>
    static bool process(ErrorType error) {
        if (error != (ErrorType)0) throw std::runtime_error(std::string() + (char)error);
        return true;
    }
};

template<std::ios_base::openmode OpenMode>
using FileDevice = Device<
    LineFileStreamHandleAccessor<OpenMode>,
    ThrowErrorHandler,
    MessageStringConverter
>;

using FileWriterDevice = FileDevice<std::ios_base::out | std::ios_base::trunc>;
using FileReaderDevice = FileDevice<std::ios_base::in>;

int main() {
    FileWriterDevice output_device("output.txt");
    output_device.open();
    output_device.write(Message { .id = 11, .len = 4, .data = {3, 2, 1, 0 }});
    output_device.write(Message { .id = 55, .len = 8, .data = {3, 2, 1, 2, 3, 4, 17, 10 }});
    output_device.close();

    Message message;
    FileReaderDevice input_device("output.txt");
    input_device.open();
    input_device.read(message);
    std::cout << message << std::endl;
    input_device.read(message);
    std::cout << message << std::endl;
    input_device.close();
}