#include "../structures/BPlusTree.cpp"
#include "../structures/InvertedIndex.cpp"
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

enum class DataType { INT, DOUBLE, STRING };

using Cell = std::variant<int, double, std::string>;

struct Column {
  std::string name;
  DataType type;
};

struct IndexEntry {
  Cell key;
  int rowId;

  bool operator<(const IndexEntry &other) const {

    if (key.index() != other.key.index()) {
      throw std::runtime_error("IndexEntry::operator<: type mismatch");
    }
    if (std::holds_alternative<int>(key)) {
      return std::get<int>(key) < std::get<int>(other.key);
    }
    if (std::holds_alternative<double>(key)) {
      return std::get<double>(key) < std::get<double>(other.key);
    }

    return std::get<std::string>(key) < std::get<std::string>(other.key);
  }

  bool operator==(const IndexEntry &other) const {
    return key == other.key && rowId == other.rowId;
  }
};

class Table {
public:
  Table(const std::vector<std::pair<std::string, DataType>> &cols,
        int hashCapacity = 101);

  ~Table();

  void insertRow(const std::vector<Cell> &row);

  void printSchema() const;

  void printAllRows() const;

  Cell getCell(int rowIndex, const std::string &colName);

  void createBPlusTreeIndex(const std::string &colName, int degree);

  void createInvertedIndex(const std::string &colName);

  std::vector<int> searchByIndex(const std::string &colName, const Cell &key);

private:
  std::vector<Column> schema_;
  HashMap<std::string, int> nameToIndex_;
  std::vector<std::vector<Cell>> data_;

  HashMap<std::string, BPlusTree<IndexEntry> *> bptIndices_;

  HashMap<std::string, InvertedIndex *> invIndices_;

  static bool cellMatchesType(const Cell &cell, DataType dt);

  static std::string dataTypeToString(DataType dt);

  int columnIndex(const std::string &colName);
};
