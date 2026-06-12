#pragma once
#include "Model.h"
#include <vector>

class Generator {
public:
    std::vector<Record> generate(int n, const std::vector<FieldDef>& fields, int startId);
};
