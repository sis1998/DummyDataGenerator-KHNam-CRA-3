#pragma once

#include "Model.h"
#include <string>
#include <vector>

class ConfigLoader {
public:
    static std::vector<FieldDef> load(const std::string& path);
};
