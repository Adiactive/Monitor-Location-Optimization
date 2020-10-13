//
// Created by Rongzhi Gu on 2019/10/26.
//

#ifndef ECE650_A2_ERROR_H
#define ECE650_A2_ERROR_H

#include <stdexcept>
//for error handling
struct Exception : std::runtime_error {
    explicit Exception(const char *msg) : runtime_error(msg) {}
};

#endif //ECE650_A2_ERROR_H
