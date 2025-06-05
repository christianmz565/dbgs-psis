#include "HashMap.cpp"
#include <string>
#include <vector>

class InvertedIndex {
public:
  InvertedIndex(int initialCapacity = 101);
  ~InvertedIndex();

  void add(const std::string &key, int rowId);
  std::vector<int> get(const std::string &key);
  void remove(const std::string &key, int rowId);
  void clear();

private:
  HashMap<std::string, std::vector<int>> map_;
};
