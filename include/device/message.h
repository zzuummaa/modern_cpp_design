#ifndef MODERN_CPP_DESIGN_MESSAGE_H
#define MODERN_CPP_DESIGN_MESSAGE_H

#include "error_type.h"
#include <sstream>
#include <iomanip>
#include <cstring>

struct Message {
    int id;
    int len;
    unsigned char data[64];
};

inline int hex_to_dec(char c) {
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

    static std::pair<ErrorType, const char*> convert(const FirstType& first, SecondType& second) {
        std::stringstream ss;
        ss << first.id << " " << first.len << " ";
        ss << std::hex;

        for(int i = 0; i < std::min(std::size(first.data), static_cast<std::size_t>(first.len)); ++i ) {
            ss << std::setw(2) << std::setfill('0') << (int)first.data[i];
        }

        if (ss.fail()) return make_error_pair(ErrorType::ConvertError);

        second = std::move(ss.str());
        return make_error_pair(ErrorType::None);
    }

    static ErrorPair convert(const SecondType& second, FirstType& first) {
        std::istringstream ss(second);
        ss >> first.id >> first.len;

        if (first.len > std::size(first.data)) return make_error_pair(ErrorType::ConvertError);
        std::memset(first.data, 0, first.len * sizeof(first.data[0]));

        char c;

        int i = 0;
        while (ss >> c) {
            if (i / 2 >= std::size(first.data)) return make_error_pair(ErrorType::ConvertError);

            int d = hex_to_dec(c);
            if (d < 0) return make_error_pair(ErrorType::ConvertError);
            first.data[i / 2] |= i % 2 ? d : d << 4;
            i++;
        }
        if (i % 2 != 0) return make_error_pair(ErrorType::ConvertError);

        bool is_ok = (ss.rdstate() & std::ios_base::eofbit) && !(ss.rdstate() & std::ios_base::failbit);
        return make_error_pair_if(!is_ok, ErrorType::ConvertError);
    }
};

std::ostream& operator<<(std::ostream& os, const Message& message) {
    MessageStringConverter::SecondType native_message;
    MessageStringConverter::convert(message, native_message);
    os << native_message;
    return os;
}

#endif //MODERN_CPP_DESIGN_MESSAGE_H
