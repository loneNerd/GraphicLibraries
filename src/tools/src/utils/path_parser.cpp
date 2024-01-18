#include "tools/utils/path_parser.hpp"

#include <algorithm>

namespace Utils = Engine::Tools::Utils;

std::string Utils::PathParser::GetFileName(const std::filesystem::path& path)
{
    if (!path.has_filename())
        return "";

    std::filesystem::path filename = path.filename();
    return filename.replace_extension("").string();
}
