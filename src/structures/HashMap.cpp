#include "HashMap.hpp"
#include <iostream>
#include <string>
using namespace std;

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

  dummy = new HashNode<K, V>(-1, -1);
}

template <typename K, typename V> HashMap<K, V>::~HashMap() {
  for (int i = 0; i < capacity; i++) {
    if (table[i] != NULL) {
      delete table[i];
    }
  }
  delete[] table;
  delete dummy;
}

template <typename K, typename V> void HashMap<K, V>::_grow() {
  HashNode<K, V> **oldArr = toList();
  capacity *= 2;
  HashNode<K, V> **newArr = new HashNode<K, V> *[capacity];
  for (int i = 0; i < capacity; i++) {
    HashNode<K, V> curr = newArr[i];
    insert(curr.key, curr->value);
  }
}

template <typename K, typename V>
void HashMap<K, V>::_insert(K key, V value, HashNode<K, V> **arr) {
  HashNode<K, V> *temp = new HashNode<K, V>(key, value);

  int hash = _hashCode(key);

  int i = 0;
  while (arr[hash] != NULL && arr[hash]->key != key &&
         arr[hash]->key != -1) {
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

template <typename K, typename V> int HashMap<K, V>::_hashCode(K key) {
  long long mod = 10e9 + 9;
  long long hash = 0;
  long long pow = 1;
  int i = 0;
  for (char c : std::to_string(key)) {
    hash = (hash + c * pow) % mod;
    i++;
    pow = pows[i];
  }
  return _wrap(hash, capacity);
}

template<typename K, typename V> double HashMap<K, V>::_getLoadFactor() {
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

template <typename K, typename V> V HashMap<K, V>::remove(K key) {

  int hashIndex = _hashCode(key);

  int i = 0;
  while (table[hashIndex] != NULL) {
    if (table[hashIndex]->key == key) {
      HashNode<K, V> *temp = table[hashIndex];

      table[hashIndex] = dummy;

      size--;
      return temp->value;
    }
    hashIndex = _wrap(hashIndex + (++i * i + i) / 2, capacity);
  }

  return NULL;
}

template <typename K, typename V> V HashMap<K, V>::get(K key) {
  int hashIndex = _hashCode(key);

  int i = 0;
  while (table[hashIndex] != NULL) {
    if (table[hashIndex]->key == key)
      return table[hashIndex]->value;
    hashIndex = _wrap(hashIndex + (++i * i + i) / 2, capacity);
  }

  return NULL;
}

template <typename K, typename V> int HashMap<K, V>::getSize() { return size; }

template <typename K, typename V> bool HashMap<K, V>::isEmpty() {
  return size == 0;
}

template <typename K, typename V> HashNode<K, V> **HashMap<K, V>::toList() {
  HashNode<K, V> **list = new HashNode<K, V> *[size];
  int index = 0;
  for (int i = 0; i < capacity; i++) {
    if (table[i] != NULL && table[i]->key != -1) {
      list[index++] = table[i];
    }
  }
  return list;
}

template <typename K, typename V> void HashMap<K, V>::display() {
  for (int i = 0; i < capacity; i++) {
    if (table[i] != NULL && table[i]->key != -1)
      cout << "key = " << table[i]->key << "  value = " << table[i]->value << endl;
  }
}
