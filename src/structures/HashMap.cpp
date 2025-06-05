#include "HashMap.hpp"
#include <iostream>
using namespace std;

template <typename K, typename V> HashNode<K, V>::HashNode(K key, V value) {
  this->value = value;
  this->key = key;
}

template <typename K, typename V> HashMap<K, V>::HashMap(int cap) {
  capacity = cap;
  size = 0;
  arr = new HashNode<K, V> *[capacity];

  for (int i = 0; i < capacity; i++)
    arr[i] = NULL;

  dummy = new HashNode<K, V>(-1, -1);
}

template <typename K, typename V> HashMap<K, V>::~HashMap() {
  for (int i = 0; i < capacity; i++) {
    if (arr[i] != NULL) {
      delete arr[i];
    }
  }
  delete[] arr;
  delete dummy;
}

template <typename K, typename V> int HashMap<K, V>::_hashCode(K key) {
  return key % capacity;
}

template <typename K, typename V> void HashMap<K, V>::insert(K key, V value) {
  HashNode<K, V> *temp = new HashNode<K, V>(key, value);

  int hashIndex = _hashCode(key);

  while (arr[hashIndex] != NULL && arr[hashIndex]->key != key &&
         arr[hashIndex]->key != -1) {
    hashIndex++;
    hashIndex %= capacity;
  }

  if (arr[hashIndex] == NULL || arr[hashIndex]->key == -1)
    size++;
  arr[hashIndex] = temp;
}

// TODO check
template <typename K, typename V> bool HashMap<K, V>::containsKey(K key) {

  int hashIndex = _hashCode(key);

  while (arr[hashIndex] != NULL) {
    if (arr[hashIndex]->key == key)
      return true;
    hashIndex++;
    hashIndex %= capacity;
  }

  return false;
}

template <typename K, typename V> V HashMap<K, V>::remove(K key) {

  int hashIndex = _hashCode(key);

  while (arr[hashIndex] != NULL) {

    if (arr[hashIndex]->key == key) {
      HashNode<K, V> *temp = arr[hashIndex];

      arr[hashIndex] = dummy;

      size--;
      return temp->value;
    }
    hashIndex++;
    hashIndex %= capacity;
  }

  return NULL;
}

template <typename K, typename V> V HashMap<K, V>::get(K key) {

  int hashIndex = _hashCode(key);
  int counter = 0;

  while (arr[hashIndex] != NULL) {
    int counter = 0;
    if (counter++ > capacity)
      return NULL;

    if (arr[hashIndex]->key == key)
      return arr[hashIndex]->value;
    hashIndex++;
    hashIndex %= capacity;
  }

  return NULL;
}

template <typename K, typename V> int HashMap<K, V>::getSize() { return size; }

template <typename K, typename V> bool HashMap<K, V>::isEmpty() {
  return size == 0;
}

template <typename K, typename V> void HashMap<K, V>::display() {
  for (int i = 0; i < capacity; i++) {
    if (arr[i] != NULL && arr[i]->key != -1)
      cout << "key = " << arr[i]->key << "  value = " << arr[i]->value << endl;
  }
}
