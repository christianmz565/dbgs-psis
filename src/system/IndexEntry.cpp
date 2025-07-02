#include "IndexEntry.hpp"
#include <stdexcept>

/**
 * @file IndexEntry.cpp
 * @brief Implementación de la estructura IndexEntry para índices de tablas.
 */

bool IndexEntry::operator<(const IndexEntry &other) const {
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

bool IndexEntry::operator>(const IndexEntry &other) const {
  return other < *this;
}

bool IndexEntry::operator<=(const IndexEntry &other) const {
  return !(*this > other);
}

bool IndexEntry::operator>=(const IndexEntry &other) const {
  return !(*this < other);
}

bool IndexEntry::operator==(const IndexEntry &other) const {
  return key == other.key && rowId == other.rowId;
}
