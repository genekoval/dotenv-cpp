#pragma once

namespace dotenv {
    constexpr auto default_file = ".env";

    auto load(const char* file = default_file) -> void;
}
