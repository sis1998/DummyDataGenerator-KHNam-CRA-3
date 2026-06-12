#include "ConsoleUI.h"
#include "Generator.h"
#include <iostream>
#include <limits>
#include <string>

static std::string findIdField(const std::vector<FieldDef>& fields) {
    for (const auto& f : fields) {
        if (f.type == "int") return f.name;
    }
    return {};
}

void ConsoleUI::run(const std::vector<FieldDef>& fields, Repository& repo) {
    if (repo.hasRecords()) {
        std::cout << "기존 데이터 " << repo.count() << "건이 있습니다.\n";
        std::cout << "추가하시겠습니까? (1: 추가 / 2: 초기화): ";

        int choice = 0;
        if (std::cin >> choice && choice == 2) {
            repo.clear();
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    const std::string idField = findIdField(fields);
    Generator gen;

    while (true) {
        std::cout << "\n=== 더미 데이터 생성기 ===\n";
        std::cout << "1. 더미 데이터 생성\n";
        std::cout << "0. 종료\n";
        std::cout << "선택: ";

        int menu = -1;
        if (!(std::cin >> menu)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "숫자를 입력해주세요.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (menu == 0) {
            break;
        } else if (menu == 1) {
            std::cout << "생성할 수를 입력하세요: ";
            int n = 0;
            if (!(std::cin >> n)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "숫자를 입력해주세요.\n";
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            int startId = idField.empty()
                ? 1
                : repo.lastId(idField) + 1;

            auto records = gen.generate(n, fields, startId);
            repo.append(records);
            std::cout << n << "건 생성 완료. (총 " << repo.count() << "건)\n";
        } else {
            std::cout << "잘못된 입력입니다.\n";
        }
    }
}
