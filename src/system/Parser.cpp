/**
 * @file Parser.cpp
 * @brief Implementación del analizador SQL simple con sintaxis de pipas.
 */

#include "Parser.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>

Parser::Parser() {}

Parser::~Parser() {}

bool Parser::executeCommand(const std::string &command) {
  try {
    ParsedCommand cmd = parseCommand(command);

    switch (cmd.type) {
    case CommandType::CREATE_TABLE:
      return executeCreateTable(cmd);
    case CommandType::INSERT:
      return executeInsert(cmd);
    case CommandType::SELECT:
      return executeSelect(cmd);
    case CommandType::DELETE:
      return executeDelete(cmd);
    case CommandType::CREATE_INDEX:
      return executeCreateIndex(cmd);
    case CommandType::CREATE_INVERTED_INDEX:
      return executeCreateInvertedIndex(cmd);
    case CommandType::SHOW_SCHEMA:
      return executeShowSchema(cmd);
    case CommandType::SHOW_TABLES:
      return executeShowTables();
    default:
      std::cout << "Unknown or unsupported command" << std::endl;
      return false;
    }
  } catch (const std::exception &e) {
    std::cout << "Error executing command: " << e.what() << std::endl;
    return false;
  }
}

ParsedCommand Parser::parseCommand(const std::string &command) {
  ParsedCommand cmd;
  cmd.type = CommandType::UNKNOWN;

  std::vector<std::string> parts = split(command, '|');
  if (parts.empty()) {
    return cmd;
  }

  std::string firstPart = trim(parts[0]);
  std::string upperFirst = toUpper(firstPart);

  if (upperFirst.find("CREATE TABLE") == 0) {
    cmd.type = CommandType::CREATE_TABLE;

    std::vector<std::string> tokens = split(firstPart, ' ');
    if (tokens.size() >= 3) {
      cmd.tableName = trim(tokens[2]);
    }

    if (parts.size() > 1) {
      std::vector<std::string> colDefs = split(parts[1], ' ');
      for (const std::string &colDef : colDefs) {
        std::string trimmedDef = trim(colDef);
        if (!trimmedDef.empty()) {
          cmd.columns.push_back(trimmedDef);
        }
      }
    }
  } else if (upperFirst.find("INSERT") == 0) {
    cmd.type = CommandType::INSERT;

    std::vector<std::string> tokens = split(firstPart, ' ');
    if (tokens.size() >= 2) {
      if (toUpper(tokens[1]) == "INTO" && tokens.size() >= 3) {
        cmd.tableName = trim(tokens[2]);
      } else {
        cmd.tableName = trim(tokens[1]);
      }
    }

    if (parts.size() > 1) {
      std::vector<std::string> values = split(trim(parts[1]), ' ', '\'');

      for (const std::string &value : values) {
        std::string trimmedValue = trim(value);
        if (!trimmedValue.empty()) {
          cmd.values.push_back(trimmedValue);
        }
      }
    }
  } else if (upperFirst.find("SELECT") == 0) {
    cmd.type = CommandType::SELECT;

    std::vector<std::string> tokens = split(firstPart, ' ');
    for (size_t i = 0; i < tokens.size(); ++i) {
      if (toUpper(tokens[i]) == "FROM" && i + 1 < tokens.size()) {
        cmd.tableName = trim(tokens[i + 1]);
        break;
      }
    }

    if (parts.size() > 1) {
      std::string wherePart = trim(parts[1]);
      if (toUpper(wherePart).find("WHERE") == 0) {
        std::vector<std::string> whereTokens = split(wherePart, ' ', '\'');
        if (whereTokens.size() >= 4) {
          cmd.whereColumn = trim(whereTokens[1]);
          cmd.whereValue = trim(whereTokens[3]);
        }
      }
    }
  } else if (upperFirst.find("DELETE") == 0) {
    cmd.type = CommandType::DELETE;

    std::vector<std::string> tokens = split(firstPart, ' ');
    for (size_t i = 0; i < tokens.size(); ++i) {
      if (toUpper(tokens[i]) == "FROM" && i + 1 < tokens.size()) {
        cmd.tableName = trim(tokens[i + 1]);
        break;
      }
    }

    if (parts.size() > 1) {
      std::string wherePart = trim(parts[1]);
      if (toUpper(wherePart).find("WHERE") == 0) {
        std::vector<std::string> whereTokens = split(wherePart, ' ', '\'');
        if (whereTokens.size() >= 4) {
          cmd.whereColumn = trim(whereTokens[1]);
          cmd.whereValue = trim(whereTokens[3]);
        }
      }
    }
  } else if (upperFirst.find("CREATE INDEX") == 0) {
    cmd.type = CommandType::CREATE_INDEX;

    std::vector<std::string> tokens = split(firstPart, ' ');
    for (size_t i = 0; i < tokens.size(); ++i) {
      if (toUpper(tokens[i]) == "ON" && i + 1 < tokens.size()) {
        cmd.tableName = trim(tokens[i + 1]);
        break;
      }
    }

    if (parts.size() > 1) {
      std::vector<std::string> indexTokens = split(trim(parts[1]), ' ');
      if (!indexTokens.empty()) {
        cmd.indexColumn = trim(indexTokens[0]);
        if (indexTokens.size() > 1) {
          try {
            cmd.indexDegree = std::stoi(trim(indexTokens[1]));
          } catch (...) {
            cmd.indexDegree = 3;
          }
        }
      }
    }
  } else if (upperFirst.find("CREATE INVERTED INDEX") == 0) {
    cmd.type = CommandType::CREATE_INVERTED_INDEX;

    std::vector<std::string> tokens = split(firstPart, ' ');
    for (size_t i = 0; i < tokens.size(); ++i) {
      if (toUpper(tokens[i]) == "ON" && i + 1 < tokens.size()) {
        cmd.tableName = trim(tokens[i + 1]);
        break;
      }
    }

    if (parts.size() > 1) {
      cmd.indexColumn = trim(parts[1]);
    }
  } else if (upperFirst.find("SHOW SCHEMA") == 0) {
    cmd.type = CommandType::SHOW_SCHEMA;
    std::vector<std::string> tokens = split(firstPart, ' ');
    if (tokens.size() >= 3) {
      cmd.tableName = trim(tokens[2]);
    }
  } else if (upperFirst.find("SHOW TABLES") == 0) {
    cmd.type = CommandType::SHOW_TABLES;
  }

  return cmd;
}

Table *Parser::getTable(const std::string &tableName) {
  auto it = tables_.find(tableName);
  if (it != tables_.end()) {
    return it->second.get();
  }
  return nullptr;
}

std::vector<std::string> Parser::listTables() const {
  std::vector<std::string> tableNames;
  for (const auto &pair : tables_) {
    tableNames.push_back(pair.first);
  }
  return tableNames;
}

bool Parser::executeCreateTable(const ParsedCommand &cmd) {
  if (cmd.tableName.empty()) {
    std::cout << "Error: Table name is required" << std::endl;
    return false;
  }

  if (tables_.find(cmd.tableName) != tables_.end()) {
    std::cout << "Error: Table '" << cmd.tableName << "' already exists"
              << std::endl;
    return false;
  }

  std::vector<std::pair<std::string, DataType>> columns;
  for (const std::string &colDef : cmd.columns) {
    size_t colonPos = colDef.find(':');
    if (colonPos == std::string::npos) {
      std::cout << "Error: Invalid column definition '" << colDef << "'"
                << std::endl;
      return false;
    }

    std::string colName = trim(colDef.substr(0, colonPos));
    std::string colType = trim(colDef.substr(colonPos + 1));

    if (colName.empty() || colType.empty()) {
      std::cout << "Error: Invalid column definition '" << colDef << "'"
                << std::endl;
      return false;
    }

    DataType dataType = parseDataType(colType);
    columns.push_back({colName, dataType});
  }

  if (columns.empty()) {
    std::cout << "Error: At least one column is required" << std::endl;
    return false;
  }

  try {
    tables_[cmd.tableName] = std::make_unique<Table>(columns);
    std::cout << "Table '" << cmd.tableName << "' created successfully"
              << std::endl;
    return true;
  } catch (const std::exception &e) {
    std::cout << "Error creating table: " << e.what() << std::endl;
    return false;
  }
}

bool Parser::executeInsert(const ParsedCommand &cmd) {
  Table *table = getTable(cmd.tableName);
  if (!table) {
    std::cout << "Error: Table '" << cmd.tableName << "' not found"
              << std::endl;
    return false;
  }

  std::vector<Cell> row;
  const std::vector<DataType> &schema = table->getSchema();
  if (cmd.values.size() != schema.size()) {
    std::cout << "Error: Value count does not match table schema" << std::endl;
    return false;
  }

  for (size_t i = 0; i < cmd.values.size(); ++i) {
    const std::string &value = cmd.values[i];
    DataType type = schema[i];
    Cell cell = parseValue(value, type);
    row.push_back(cell);
  }

  try {
    table->insertRow(row);
    std::cout << "Row inserted successfully" << std::endl;
    return true;
  } catch (const std::exception &e) {
    std::cout << "Error inserting row: " << e.what() << std::endl;
    return false;
  }
}

bool Parser::executeSelect(const ParsedCommand &cmd) {
  Table *table = getTable(cmd.tableName);
  if (!table) {
    std::cout << "Error: Table '" << cmd.tableName << "' not found"
              << std::endl;
    return false;
  }

  if (cmd.whereColumn.empty()) {

    table->printAllRows();
  } else {

    try {
      DataType columnType = table->getColumnType(cmd.whereColumn);
      Cell searchValue = parseValue(cmd.whereValue, columnType);
      std::vector<int> rowIndices =
          table->search(cmd.whereColumn, searchValue);

      if (rowIndices.empty()) {
        std::cout << "No rows found matching criteria" << std::endl;
      } else {
        for (int rowIndex : rowIndices) {
          std::vector<Cell> &row = table->getRow(rowIndex);
          for (const Cell &cell : row) {
            std::visit([](const auto &value) { std::cout << value << " "; },
                       cell);
          }
          std::cout << std::endl;
        }
      }
    } catch (const std::exception &e) {
      std::cout << "Error executing select: " << e.what() << std::endl;
      return false;
    }
  }

  return true;
}

bool Parser::executeDelete(const ParsedCommand &cmd) {
  Table *table = getTable(cmd.tableName);
  if (!table) {
    std::cout << "Error: Table '" << cmd.tableName << "' not found"
              << std::endl;
    return false;
  }

  if (cmd.whereColumn.empty() || cmd.whereValue.empty()) {
    std::cout << "Error: DELETE requires WHERE clause" << std::endl;
    return false;
  }

  try {
    DataType columnType = table->getColumnType(cmd.whereColumn);
    Cell searchValue = parseValue(cmd.whereValue, columnType);
    std::vector<int> rowIndices =
        table->search(cmd.whereColumn, searchValue);

    std::sort(rowIndices.rbegin(), rowIndices.rend());

    int deletedCount = 0;
    for (int rowIndex : rowIndices) {
      table->deleteRow(rowIndex);
      deletedCount++;
    }

    std::cout << "Deleted " << deletedCount << " row(s)" << std::endl;
    return true;
  } catch (const std::exception &e) {
    std::cout << "Error executing delete: " << e.what() << std::endl;
    return false;
  }
}

bool Parser::executeCreateIndex(const ParsedCommand &cmd) {
  Table *table = getTable(cmd.tableName);
  if (!table) {
    std::cout << "Error: Table '" << cmd.tableName << "' not found"
              << std::endl;
    return false;
  }

  if (cmd.indexColumn.empty()) {
    std::cout << "Error: Column name is required for index creation"
              << std::endl;
    return false;
  }

  try {
    table->createBPlusTreeIndex(cmd.indexColumn, cmd.indexDegree);
    std::cout << "B+ Tree index created on column '" << cmd.indexColumn
              << "' with degree " << cmd.indexDegree << std::endl;
    return true;
  } catch (const std::exception &e) {
    std::cout << "Error creating index: " << e.what() << std::endl;
    return false;
  }
}

bool Parser::executeCreateInvertedIndex(const ParsedCommand &cmd) {
  Table *table = getTable(cmd.tableName);
  if (!table) {
    std::cout << "Error: Table '" << cmd.tableName << "' not found"
              << std::endl;
    return false;
  }

  if (cmd.indexColumn.empty()) {
    std::cout << "Error: Column name is required for inverted index creation"
              << std::endl;
    return false;
  }

  try {
    table->createInvertedIndex(cmd.indexColumn);
    std::cout << "Inverted index created on column '" << cmd.indexColumn << "'"
              << std::endl;
    return true;
  } catch (const std::exception &e) {
    std::cout << "Error creating inverted index: " << e.what() << std::endl;
    return false;
  }
}

bool Parser::executeShowSchema(const ParsedCommand &cmd) {
  Table *table = getTable(cmd.tableName);
  if (!table) {
    std::cout << "Error: Table '" << cmd.tableName << "' not found"
              << std::endl;
    return false;
  }

  table->printSchema();
  return true;
}

bool Parser::executeShowTables() {
  std::vector<std::string> tableNames = listTables();
  if (tableNames.empty()) {
    std::cout << "No tables found" << std::endl;
  } else {
    std::cout << "Tables:" << std::endl;
    for (const std::string &name : tableNames) {
      std::cout << "  " << name << std::endl;
    }
  }
  return true;
}

DataType Parser::parseDataType(const std::string &typeStr) {
  std::string upperType = toUpper(typeStr);
  if (upperType == "INT" || upperType == "INTEGER") {
    return DataType::INT;
  } else if (upperType == "DOUBLE" || upperType == "FLOAT") {
    return DataType::DOUBLE;
  } else if (upperType == "STRING" || upperType == "TEXT") {
    return DataType::STRING;
  } else {
    throw std::invalid_argument("Unknown data type: " + typeStr);
  }
}

Cell Parser::parseValue(const std::string &valueStr, DataType dataType) {
  switch (dataType) {
  case DataType::INT:
    return std::stoi(valueStr);
  case DataType::DOUBLE:
    return std::stod(valueStr);
  case DataType::STRING:
  default:
    return valueStr;
  }
}

std::vector<std::string> Parser::split(const std::string &str, char delimiter,
                                       char escapeChar) {
  std::vector<std::string> tokens;
  std::string token;
  bool inEscape = false;

  if (escapeChar == '\0') {
    for (size_t i = 0; i < str.size(); ++i) {
      char c = str[i];
      if (c == delimiter) {
        tokens.push_back(token);
        token.clear();
      } else {
        token += c;
      }
    }
    tokens.push_back(token);
    return tokens;
  }

  for (size_t i = 0; i < str.size(); ++i) {
    char c = str[i];
    if (c == escapeChar) {
      inEscape = !inEscape;
      continue;
    }
    if (c == delimiter && !inEscape) {
      tokens.push_back(token);
      token.clear();
    } else {
      token += c;
    }
  }
  tokens.push_back(token);
  return tokens;
}

std::string Parser::trim(const std::string &str) {
  size_t start = str.find_first_not_of(" \t\n\r");
  if (start == std::string::npos) {
    return "";
  }

  size_t end = str.find_last_not_of(" \t\n\r");
  return str.substr(start, end - start + 1);
}

std::string Parser::toUpper(const std::string &str) {
  std::string result = str;
  std::transform(result.begin(), result.end(), result.begin(), ::toupper);
  return result;
}
