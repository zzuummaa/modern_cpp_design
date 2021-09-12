#ifndef MODERN_CPP_DESIGN_DEVICE_H
#define MODERN_CPP_DESIGN_DEVICE_H

template<typename IMessageType>
class IDevice {
public:
    virtual bool open() = 0;
    virtual bool write(const IMessageType&) = 0;
    virtual bool read(IMessageType& message) = 0;
    virtual bool close() = 0;
};

#endif //MODERN_CPP_DESIGN_DEVICE_H
