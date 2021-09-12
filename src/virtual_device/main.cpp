#include <device/fstream_device.h>
#include <iostream>

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