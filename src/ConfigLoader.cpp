#include "ConfigLoader.h"
#include "../vendor/json.hpp"

#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

std::vector<FieldDef> ConfigLoader::load(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("설정 파일을 찾을 수 없습니다: " + path);

    json root;
    try {
        file >> root;
    }
    catch (const json::exception& e) {
        throw std::runtime_error("설정 파일 파싱 실패: " + std::string(e.what()));
    }

    std::vector<FieldDef> fields;
    for (const auto& item : root.at("fields")) {
        FieldDef fd;
        fd.name   = item.at("name").get<std::string>();
        fd.type   = item.at("type").get<std::string>();
        fd.prefix = item.at("prefix").get<std::string>();
        fields.push_back(std::move(fd));
    }
    return fields;
}
