#include "Calculator.h"
#include <boost/numeric/conversion/cast.hpp>
#include <limits>
#include <stdexcept>

Calculator::Calculator(std::vector<int32_t> input_data) {
    try {
        int64_t temp_res = 1;
        for (auto elem : input_data) {
            if (temp_res > std::numeric_limits<int32_t>::max() / elem) {
                throw boost::numeric::positive_overflow();
            }
            temp_res *= elem;
        }
        if (temp_res > std::numeric_limits<int32_t>::max()) {
            results = std::numeric_limits<int32_t>::max();
        } else {
            results = static_cast<int32_t>(temp_res);
        }
    } catch (boost::numeric::positive_overflow&) {
        results = std::numeric_limits<int32_t>::max();
    }
}

int32_t Calculator::send_res() {
    return results;
}
