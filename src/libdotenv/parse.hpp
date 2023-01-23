#pragma once

#include <array>
#include <sstream>

namespace dotenv::detail {
    auto parse(std::istream& stream) -> void;

    auto split(std::string_view line) -> std::array<std::string_view, 2>;

    auto trim(std::string_view string) -> std::string_view;

    auto set(std::string_view name, std::string_view value) -> void;

    auto unset(std::string_view name) -> void;
}
