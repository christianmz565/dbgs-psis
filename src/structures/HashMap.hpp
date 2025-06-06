#include <iostream>
#include <optional>
#include <sstream>
#include <string>

template <typename K, typename V> class HashNode {
public:
  V value;
  K key;

  HashNode(K key, V value);
};

template <typename K, typename V> class HashMap {
  HashNode<K, V> **table;
  int capacity;
  int size;
  long long *pows;

public:
  HashMap(int cap);
  ~HashMap();

  long long *_getPows();
  double _getLoadFactor();
  std::string _toStringGeneric(const K &val);
  int _hashCode(K key);
  int _wrap(long long key, int size);
  void _insert(K key, V value, HashNode<K, V> **arr);
  void _grow();

  void insert(K key, V value);
  std::optional<V> get(K key);
  std::optional<V> remove(K key);
  bool containsKey(K key);
  int getSize();
  bool isEmpty();
  HashNode<K, V> **toList();
  void display();
  void clear();
};

template <typename K, typename V> HashNode<K, V>::HashNode(K key, V value) {
  this->value = value;
  this->key = key;
}

template <typename K, typename V> HashMap<K, V>::HashMap(int cap) {
  capacity = cap;
  size = 0;
  table = new HashNode<K, V> *[capacity];

  for (int i = 0; i < capacity; i++)
    table[i] = NULL;

  pows = _getPows();
}

template <typename K, typename V> HashMap<K, V>::~HashMap() {
  for (int i = 0; i < capacity; i++) {
    if (table[i] != NULL) {
      delete table[i];
    }
  }
  delete[] table;
}

template <typename K, typename V> void HashMap<K, V>::_grow() {
  HashNode<K, V> **oldTable = toList();
  capacity *= 2;
  HashNode<K, V> **newTable = new HashNode<K, V> *[capacity];
  for (int i = 0; i < capacity; i++) {
    HashNode<K, V> curr = *oldTable[i];
    _insert(curr.key, curr.value, newTable);
  }
}

template <typename K, typename V>
void HashMap<K, V>::_insert(K key, V value, HashNode<K, V> **arr) {
  HashNode<K, V> *temp = new HashNode<K, V>(key, value);

  int hash = _hashCode(key);

  int i = 0;
  while (arr[hash] != NULL && arr[hash]->key != key) {
    hash = _wrap(hash + (++i * i + i) / 2, capacity);
  }

  arr[hash] = temp;
}

template <typename K, typename V> long long *HashMap<K, V>::_getPows() {
  long long mod = 10e9 + 7;
  long long pow = 1;
  long long *pows = new long long[25];
  for (int i = 0; i < 25; i++) {
    pows[i] = pow;
    pow = (pow * 5503) % mod;
  }
  return pows;
}

template <typename K, typename V>
int HashMap<K, V>::_wrap(long long key, int size) {
  key %= size;
  if (key < 0)
    key += size;
  return key;
}

template <typename K, typename V>
std::string HashMap<K, V>::_toStringGeneric(const K &val) {
  std::ostringstream oss;
  oss << val;
  return oss.str();
}

template <typename K, typename V> int HashMap<K, V>::_hashCode(K key) {
  long long mod = 10e9 + 9;
  long long hash = 0;
  long long pow = 1;
  int i = 0;
  for (char c : _toStringGeneric(key)) {
    hash = (hash + c * pow) % mod;
    i++;
    pow = pows[i];
  }
  return _wrap(hash, capacity);
}

template <typename K, typename V> double HashMap<K, V>::_getLoadFactor() {
  return (double)size / capacity;
}

template <typename K, typename V> void HashMap<K, V>::insert(K key, V value) {
  if (_getLoadFactor() > 0.6) {
    _grow();
  }
  _insert(key, value, table);
  size++;
}

template <typename K, typename V> bool HashMap<K, V>::containsKey(K key) {

  int hash = _hashCode(key);

  int i = 0;
  while (table[hash] != NULL) {
    if (table[hash]->key == key)
      return true;
    hash = _wrap(hash + (++i * i + i) / 2, capacity);
  }

  return false;
}

template <typename K, typename V>
std::optional<V> HashMap<K, V>::remove(K key) {
  int hash = _hashCode(key);

  int i = 0;
  while (table[hash] != NULL) {
    if (table[hash]->key == key) {
      HashNode<K, V> *temp = table[hash];

      table[hash] = nullptr;

      size--;
      return temp->value;
    }
    hash = _wrap(hash + (++i * i + i) / 2, capacity);
  }
  return std::nullopt;
}

template <typename K, typename V> std::optional<V> HashMap<K, V>::get(K key) {
  int hashIndex = _hashCode(key);

  int i = 0;
  while (table[hashIndex] != NULL) {
    if (table[hashIndex]->key == key)
      return table[hashIndex]->value;
    hashIndex = _wrap(hashIndex + (++i * i + i) / 2, capacity);
  }
  return std::nullopt;
}

template <typename K, typename V> int HashMap<K, V>::getSize() { return size; }

template <typename K, typename V> bool HashMap<K, V>::isEmpty() {
  return size == 0;
}

template <typename K, typename V> HashNode<K, V> **HashMap<K, V>::toList() {
  HashNode<K, V> **list = new HashNode<K, V> *[size];
  int index = 0;
  for (int i = 0; i < capacity; i++) {
    if (table[i] != NULL) {
      list[index++] = table[i];
    }
  }
  return list;
}

template <typename K, typename V> void HashMap<K, V>::display() {
  for (int i = 0; i < capacity; i++) {
    if (table[i] != NULL)
      std::cout << "key = " << table[i]->key << "  value = " << table[i]->value
                << std::endl;
  }
}

template <typename K, typename V> void HashMap<K, V>::clear() {
  for (int i = 0; i < capacity; i++) {
    if (table[i] != NULL) {
      delete table[i];
      table[i] = NULL;
    }
  }
  size = 0;
}
