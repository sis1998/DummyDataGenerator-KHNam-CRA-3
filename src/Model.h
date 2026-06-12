#pragma once

#include <map>
#include <string>

using Record = std::map<std::string, std::string>;

struct FieldDef {
    std::string name;
    std::string type;    // "int" | "string"
    std::string prefix;  // 빈 문자열이면 인덱스 숫자만 사용
};
