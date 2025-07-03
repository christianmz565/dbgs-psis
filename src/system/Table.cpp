#include "Table.hpp"
#include <climits>
#include <iostream>
#include <sstream>
#include <stdexcept>

/** \file Table.cpp
 * Implementación de la clase Table.
 */

Table::Table(const std::vector<std::pair<std::string, DataType>> &cols,
             int hashCapacity)
    : nameToIndex_(hashCapacity), bptIndices_(hashCapacity),
      invIndices_(hashCapacity) {

  for (int i = 0; i < cols.size(); ++i) {
    const auto &[colName, colType] = cols[i];
    if (colName.empty()) {
      throw std::invalid_argument("Column name cannot be empty");
    }

    if (nameToIndex_.containsKey(colName)) {
      throw std::invalid_argument("Duplicate column name: " + colName);
    }
    schema_.push_back({colName, colType});
    nameToIndex_.insert(colName, i);
  }
}

Table::~Table() {

  for (int i = 0; i < bptIndices_.getSize(); ++i) {
  }

  for (int i = 0; i < schema_.size(); ++i) {
    const auto &colName = schema_[i].name;
    if (bptIndices_.containsKey(colName)) {
      BPlusTree<IndexEntry> *treePtr = bptIndices_.get(colName).value();
      delete treePtr;
    }
    if (invIndices_.containsKey(colName)) {
      InvertedIndex *idxPtr = invIndices_.get(colName).value();
      delete idxPtr;
    }
  }
}

bool Table::cellMatchesType(const Cell &cell, DataType dt) {
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

std::string Table::dataTypeToString(DataType dt) {
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

int Table::columnIndex(const std::string &colName) {
  if (!nameToIndex_.containsKey(colName)) {
    throw std::invalid_argument("Unknown column '" + colName + "'");
  }
  return nameToIndex_.get(colName).value();
}

void Table::printSchema() const {
  std::cout << "Schema:\n";
  for (const auto &col : schema_) {
    std::cout << "  " << col.name << " : " << dataTypeToString(col.type)
              << "\n";
  }
}

void Table::printAllRows() const {

  for (const auto &col : schema_) {
    std::cout << col.name << "\t";
  }
  std::cout << "\n----------------------------------------\n";

  for (int r = 0; r < data_.size(); ++r) {
    const auto &row = data_[r];
    for (int c = 0; c < row.size(); ++c) {
      std::visit([](auto &&val) { std::cout << val << "\t"; }, row[c]);
    }
    std::cout << "\n";
  }
}

std::vector<Cell> &Table::getRow(int rowIndex) {
  if (rowIndex < 0 || rowIndex >= data_.size()) {
    throw std::out_of_range("Row index out of range");
  }
  return data_[rowIndex];
}

std::vector<DataType> Table::getSchema() const {
  std::vector<DataType> schemaTypes;
  for (const auto &col : schema_) {
    schemaTypes.push_back(col.type);
  }
  return schemaTypes;
}

DataType &Table::getColumnType(const std::string &colName) {
  int cidx = columnIndex(colName);
  return schema_[cidx].type;
}

std::vector<Cell> &Table::deleteRow(int rowIndex) {
  if (rowIndex < 0 || rowIndex >= data_.size()) {
    throw std::out_of_range("Row index out of range");
  }

  std::vector<Cell> &deletedRow = data_[rowIndex];

  for (int i = 0; i < schema_.size(); ++i) {
    const std::string &colName = schema_[i].name;
    if (bptIndices_.containsKey(colName)) {
      BPlusTree<IndexEntry> *treePtr = bptIndices_.get(colName).value();
      IndexEntry ie{deletedRow[i], rowIndex};
      treePtr->remove(ie);
    }
  }

  for (int i = 0; i < schema_.size(); ++i) {
    if (schema_[i].type == DataType::STRING) {
      const std::string &colName = schema_[i].name;
      if (invIndices_.containsKey(colName)) {
        InvertedIndex *invPtr = invIndices_.get(colName).value();
        const std::string &keyStr = std::get<std::string>(deletedRow[i]);
        invPtr->remove(keyStr, rowIndex);
      }
    }
  }

  data_.erase(data_.begin() + rowIndex);
  return deletedRow;
}

Cell &Table::getCell(int rowIndex, const std::string &colName) {
  int cidx = columnIndex(colName);
  if (rowIndex >= data_.size()) {
    throw std::out_of_range("Row index out of range");
  }
  return data_[rowIndex][cidx];
}

void Table::insertRow(const std::vector<Cell> &row) {
  if (row.size() != schema_.size()) {
    throw std::invalid_argument("Row size does not match schema size");
  }

  for (int i = 0; i < row.size(); ++i) {
    if (!cellMatchesType(row[i], schema_[i].type)) {
      std::ostringstream oss;
      oss << "Type mismatch for column '" << schema_[i].name << "' at index "
          << i;
      throw std::invalid_argument(oss.str());
    }
  }

  int newRowId = data_.size();
  data_.push_back(row);

  for (int i = 0; i < schema_.size(); ++i) {
    const std::string &colName = schema_[i].name;
    if (bptIndices_.containsKey(colName)) {
      BPlusTree<IndexEntry> *treePtr = bptIndices_.get(colName).value();
      IndexEntry ie{row[i], newRowId};
      treePtr->insert(ie);
    }
  }

  for (int i = 0; i < schema_.size(); ++i) {
    if (schema_[i].type == DataType::STRING) {
      const std::string &colName = schema_[i].name;
      if (invIndices_.containsKey(colName)) {
        InvertedIndex *invPtr = invIndices_.get(colName).value();
        const std::string &keyStr = std::get<std::string>(row[i]);
        invPtr->add(keyStr, newRowId);
      }
    }
  }
}

void Table::createBPlusTreeIndex(const std::string &colName, int degree) {
  int cidx = columnIndex(colName);
  DataType dt = schema_[cidx].type;

  BPlusTree<IndexEntry> *treePtr = new BPlusTree<IndexEntry>(degree);

  for (int rid = 0; rid < data_.size(); ++rid) {
    const Cell &cell = data_[rid][cidx];
    IndexEntry ie{cell, rid};
    treePtr->insert(ie);
  }

  if (bptIndices_.containsKey(colName)) {
    delete bptIndices_.get(colName).value();
  }
  bptIndices_.insert(colName, treePtr);
}

void Table::createInvertedIndex(const std::string &colName) {
  int cidx = columnIndex(colName);
  if (schema_[cidx].type != DataType::STRING) {
    throw std::invalid_argument("Inverted index only valid on STRING columns");
  }

  InvertedIndex *invPtr = new InvertedIndex();

  for (int rid = 0; rid < data_.size(); ++rid) {
    const std::string &val = std::get<std::string>(data_[rid][cidx]);
    invPtr->add(val, rid);
  }

  if (invIndices_.containsKey(colName)) {
    delete invIndices_.get(colName).value();
  }
  invIndices_.insert(colName, invPtr);
}

std::vector<int> Table::searchByIndex(const std::string &colName,
                                      const Cell &key) {
  int cidx = columnIndex(colName);
  DataType dt = schema_[cidx].type;

  if (bptIndices_.containsKey(colName)) {
    BPlusTree<IndexEntry> *treePtr = bptIndices_.get(colName).value();

    int maxPossible = data_.size();
    IndexEntry *buffer = new IndexEntry[maxPossible];

    IndexEntry start{key, 0};
    IndexEntry end{key, INT_MAX};

    int foundCount = treePtr->searchRange(start, end, buffer, (int)maxPossible);

    std::vector<int> result;
    result.reserve(foundCount);
    for (int i = 0; i < foundCount; ++i) {
      result.push_back(buffer[i].rowId);
    }
    delete[] buffer;
    return result;
  }

  if (schema_[cidx].type == DataType::STRING &&
      invIndices_.containsKey(colName)) {
    InvertedIndex *invPtr = invIndices_.get(colName).value();
    const std::string &keyStr = std::get<std::string>(key);
    return invPtr->get(keyStr);
  }

  return {};
}

std::vector<int> Table::search(const std::string &colName, const Cell &key) {
  if (bptIndices_.containsKey(colName) ||
      (columnIndex(colName) >= 0 &&
       schema_[columnIndex(colName)].type == DataType::STRING &&
       invIndices_.containsKey(colName))) {
    return searchByIndex(colName, key);
  }

  int cidx = columnIndex(colName);
  std::vector<int> result;
  for (int i = 0; i < data_.size(); ++i) {
    if (data_[i][cidx] == key) {
      result.push_back(i);
    }
  }
  return result;
}
