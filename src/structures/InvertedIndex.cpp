#include "InvertedIndex.hpp"

InvertedIndex::InvertedIndex(int initialCapacity) : map_(initialCapacity) {}

InvertedIndex::~InvertedIndex() { clear(); }

void InvertedIndex::add(const std::string &key, int rowId) {
  if (map_.containsKey(key)) {
    std::vector<int> existing = map_.get(key).value();
    existing.push_back(rowId);
    map_.insert(key, existing);
  } else {
    std::vector<int> vec;
    vec.push_back(rowId);
    map_.insert(key, vec);
  }
}

std::vector<int> InvertedIndex::get(const std::string &key) {
  if (map_.containsKey(key)) {
    return map_.get(key).value();
  } else {
    return {};
  }
}

void InvertedIndex::remove(const std::string &key, int rowId) {
  if (!map_.containsKey(key))
    return;

  std::vector<int> existing = map_.get(key).value();

  std::vector<int> temp;
  for (int x : existing) {
    if (x != rowId) {
      temp.push_back(x);
    }
  }
  existing = temp;

  if (existing.empty()) {
    map_.remove(key);
  } else {
    map_.insert(key, existing);
  }
}

void InvertedIndex::clear() { map_.clear(); }
