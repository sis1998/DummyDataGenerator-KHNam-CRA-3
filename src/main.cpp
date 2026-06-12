#include "ConfigLoader.h"
#include "Repository.h"
#include "ConsoleUI.h"
#include <windows.h>
#include <iostream>
#include <stdexcept>

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    try {
        auto fields = ConfigLoader::load("data/default.json");
        Repository repo("data/records.json");
        repo.load();

        ConsoleUI::run(fields, repo);
    }
    catch (const std::exception& e) {
        std::cerr << "오류: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
