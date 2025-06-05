#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

enum class DataType { INT, DOUBLE, STRING };

using Cell = std::variant<int, double, std::string>;

struct Column {
  std::string name;
  DataType type;
};

class Table {
public:
  Table(const std::vector<std::pair<std::string, DataType>> &cols) {

    for (const auto &[colName, colType] : cols) {
      if (colName.empty()) {
        throw std::invalid_argument("Column name cannot be empty");
      }
      schema_.push_back({colName, colType});
    }

    for (size_t i = 0; i < schema_.size(); ++i) {
      nameToIndex_[schema_[i].name] = i;
    }
  }

  void insertRow(const std::vector<Cell> &row) {
    if (row.size() != schema_.size()) {
      throw std::invalid_argument("Row size does not match schema size");
    }

    for (size_t i = 0; i < row.size(); ++i) {
      if (!cellMatchesType(row[i], schema_[i].type)) {
        std::ostringstream oss;
        oss << "Type mismatch for column '" << schema_[i].name << "' at index "
            << i;
        throw std::invalid_argument(oss.str());
      }
    }
    data_.push_back(row);
  }

  void insertRow(const std::unordered_map<std::string, Cell> &rowMap) {
    std::vector<Cell> row(schema_.size());
    for (size_t i = 0; i < schema_.size(); ++i) {
      const auto &col = schema_[i];
      auto it = rowMap.find(col.name);
      if (it == rowMap.end()) {
        throw std::invalid_argument("Missing value for column '" + col.name +
                                    "'");
      }
      if (!cellMatchesType(it->second, col.type)) {
        throw std::invalid_argument("Type mismatch for column '" + col.name +
                                    "'");
      }
      row[i] = it->second;
    }
    data_.push_back(std::move(row));
  }

  void printSchema() const {
    std::cout << "Schema:\n";
    for (const auto &col : schema_) {
      std::cout << "  " << col.name << " : " << dataTypeToString(col.type)
                << "\n";
    }
  }

  void printAllRows() const {

    for (const auto &col : schema_) {
      std::cout << col.name << "\t";
    }
    std::cout << "\n----------------------------------------\n";

    for (const auto &row : data_) {
      for (const auto &cell : row) {
        std::visit([](auto &&val) { std::cout << val << "\t"; }, cell);
      }
      std::cout << "\n";
    }
  }

  Cell getCell(size_t rowIndex, const std::string &colName) const {
    auto it = nameToIndex_.find(colName);
    if (it == nameToIndex_.end()) {
      throw std::invalid_argument("Unknown column '" + colName + "'");
    }
    size_t colIndex = it->second;
    if (rowIndex >= data_.size()) {
      throw std::out_of_range("Row index out of range");
    }
    return data_[rowIndex][colIndex];
  }

  size_t rowCount() const { return data_.size(); }

  size_t colCount() const { return schema_.size(); }

private:
  std::vector<Column> schema_;
  std::unordered_map<std::string, size_t> nameToIndex_;
  std::vector<std::vector<Cell>> data_;

  static bool cellMatchesType(const Cell &cell, DataType dt) {
    switch (dt) {
    case DataType::INT:
      return std::holds_alternative<int>(cell);
    case DataType::DOUBLE:
      return std::holds_alternative<double>(cell);
    case DataType::STRING:
      return std::holds_alternative<std::string>(cell);
    }
    return false;
  }

  static std::string dataTypeToString(DataType dt) {
    switch (dt) {
    case DataType::INT:
      return "INT";
    case DataType::DOUBLE:
      return "DOUBLE";
    case DataType::STRING:
      return "STRING";
    }
    return "UNKNOWN";
  }
};

