#include "Repository.h"
#include "../vendor/json.hpp"
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

Repository::Repository(const std::string& path) : path_(path) {}

void Repository::load() {
    std::ifstream file(path_);
    if (!file.is_open()) {
        records_.clear();
        return;
    }

    try {
        json j;
        file >> j;
        records_.clear();
        for (const auto& obj : j) {
            Record record;
            for (auto it = obj.begin(); it != obj.end(); ++it) {
                record[it.key()] = it.value().get<std::string>();
            }
            records_.push_back(std::move(record));
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("데이터 파일 파싱 실패: " + std::string(e.what()));
    }
}

bool Repository::hasRecords() const {
    return !records_.empty();
}

void Repository::clear() {
    records_.clear();
}

void Repository::append(const std::vector<Record>& newRecords) {
    records_.insert(records_.end(), newRecords.begin(), newRecords.end());
    flush();
}

int Repository::lastId(const std::string& idField) const {
    if (records_.empty()) {
        return 0;
    }

    int maxId = 0;
    for (const auto& record : records_) {
        auto it = record.find(idField);
        if (it != record.end()) {
            try {
                int val = std::stoi(it->second);
                if (val > maxId) maxId = val;
            } catch (...) {}
        }
    }
    return maxId;
}

size_t Repository::count() const {
    return records_.size();
}

void Repository::flush() const {
    json j = json::array();
    for (const auto& record : records_) {
        json obj;
        for (const auto& [key, value] : record) {
            obj[key] = value;
        }
        j.push_back(obj);
    }

    std::ofstream file(path_);
    if (!file.is_open()) {
        throw std::runtime_error("데이터 파일 저장 실패: " + path_);
    }
    file << j.dump(2);
}
