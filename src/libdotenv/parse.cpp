#include "parse.hpp"

#include <dotenv/except.hpp>

#include <iostream>

namespace {
    constexpr auto npos = std::string_view::npos;

    auto is_space(unsigned char c) -> bool { return std::isspace(c); }

    auto trim_start(std::string_view string) -> std::string_view {
        std::size_t index = 0;

        while (index < string.size() && is_space(string[index])) ++index;

        return string.substr(index);
    }

    auto trim_end(std::string_view string) -> std::string_view {
        if (string.empty()) return string;

        auto index = string.size() - 1;

        while (index >= 0 && is_space(string[index])) --index;

        return string.substr(0, index + 1);
    }
}

namespace dotenv::detail {
    auto parse(std::istream& stream) -> void {
        auto line = std::string();

        for (std::size_t n = 1; std::getline(stream, line); ++n) {
            const auto trimmed = trim(line);

            if (trimmed.empty() || trimmed.starts_with('#')) continue;

            const auto [name, value] = split(trimmed);

            try {
                if (value.empty()) unset(name);
                else set(name, value);
            }
            catch (const error& ex) {
                auto what = std::ostringstream();
                what << "line " << n << ": " << ex.what();
                throw error(what.str());
            }
        }
    }

    auto set(std::string_view name, std::string_view value) -> void {
        const auto name_string = std::string(name);
        const auto value_string = std::string(value);

        if (setenv(name_string.c_str(), value_string.c_str(), 1) == -1) {
            if (errno == EINVAL) throw error("empty variable name");
            throw std::system_error(errno, std::generic_category());
        }
    }

    auto split(std::string_view line) -> std::array<std::string_view, 2> {
        const auto delim = line.find('=');

        const auto name = line.substr(0, delim);
        auto value = line.substr(delim + 1);

        if ((value.starts_with('\'') && value.ends_with('\'')) ||
            (value.starts_with('"') && value.ends_with('"')))
            value = value.substr(1, value.size() - 2);
        else {
            const auto comment = value.find(" #");
            if (comment != npos) { value = trim(value.substr(0, comment)); }
        }

        return {name, value};
    }

    auto trim(std::string_view string) -> std::string_view {
        return trim_end(trim_start(string));
    }

    auto unset(std::string_view name) -> void {
        const auto string = std::string(name);

        if (unsetenv(string.c_str()) == -1) {
            if (errno == EINVAL) throw error("empty variable name");
            throw std::system_error(errno, std::generic_category());
        }
    }
}
