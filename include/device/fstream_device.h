#ifndef MODERN_CPP_DESIGN_FSTREAM_DEVICE_H
#define MODERN_CPP_DESIGN_FSTREAM_DEVICE_H

#include "device.h"
#include "message.h"
#include "error_handler.h"
#include <fstream>

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

    static ErrorPair open(HandleType& handle, const std::string& path) {
        handle.open(path, OpenMode);
        return make_error_pair_if(!handle.is_open(), ErrorType::UnknownDevice);
    }

    static ErrorPair read(HandleType& handle, std::string& message) {
        std::getline(handle, message);
        return make_error_pair_if(!handle.good(), ErrorType::ReadError);
    }

    static ErrorPair write(HandleType& handle, std::string& message) {
        handle << message << std::endl;
        return make_error_pair_if(!handle.good(), ErrorType::WriteError);
    }

    static ErrorPair close(HandleType& handle) {
        handle.close();
        return make_error_pair_if(handle.fail(), ErrorType::UnknownError);
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

#endif //MODERN_CPP_DESIGN_FSTREAM_DEVICE_H
