#pragma once
#include "Model.h"
#include <string>
#include <vector>

class Repository {
public:
    explicit Repository(const std::string& path);

    void load();
    bool hasRecords() const;
    void clear();
    void append(const std::vector<Record>& newRecords);
    int lastId(const std::string& idField) const;
    size_t count() const;

private:
    std::string path_;
    std::vector<Record> records_;

    void flush() const;
};
