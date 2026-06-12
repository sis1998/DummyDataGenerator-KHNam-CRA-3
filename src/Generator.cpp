#include "Generator.h"
#include <stdexcept>

std::vector<Record> Generator::generate(int n, const std::vector<FieldDef>& fields, int startId) {
    if (n <= 0) {
        throw std::invalid_argument("생성 수는 1 이상이어야 합니다");
    }

    std::vector<Record> records;
    records.reserve(n);

    for (int i = 0; i < n; ++i) {
        Record record;
        for (const auto& field : fields) {
            record[field.name] = field.prefix + std::to_string(startId + i);
        }
        records.push_back(std::move(record));
    }

    return records;
}
