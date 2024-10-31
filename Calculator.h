#pragma once
#include <vector>
#include <cstdint>

class Calculator
{
    int32_t results;
public:
    Calculator(std::vector<int32_t> input_data);
    int32_t send_res();
};
