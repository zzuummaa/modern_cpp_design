#include <blocks/block.h>
#include <iostream>
#include <array>

int main() {
    std::array<int, 2> input_data = { 1, 5 };
    double output_data = 3;

    auto input_collection = std::tie(input_data[0], input_data[1]);
    auto output_collection = std::tie(output_data);

    SimpleSumBlock<double, int, int> sum_block(input_collection, output_collection);
    static_cast<IBlock&>(sum_block).calc();

    std::cout << "sum is " << output_data << std::endl;

    return 0;
}