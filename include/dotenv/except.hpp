#pragma once

#include <stdexcept>

namespace dotenv {
    class error : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}
