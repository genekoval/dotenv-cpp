#include "parse.hpp"

#include <dotenv/except.hpp>
#include <dotenv/load.hpp>

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace dotenv {
    auto load(const char* file) -> void {
        if (fs::status(file).type() != fs::file_type::regular) return;

        auto stream = std::ifstream(file);

        try {
            detail::parse(stream);
        }
        catch (const error& ex) {
            auto what = std::ostringstream();
            what << file << ": " << ex.what();
            throw error(what.str());
        }
    }
}
