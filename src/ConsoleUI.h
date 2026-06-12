#pragma once

#include "Model.h"
#include "Repository.h"
#include <vector>

class ConsoleUI {
public:
    static void run(const std::vector<FieldDef>& fields, Repository& repo);
};
