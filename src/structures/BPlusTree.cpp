#include "BPlusTree.hpp"
#include <iostream>

template <typename T> Node<T>::Node(int _degree) {
  this->is_leaf = false;
  this->degree = _degree;
  this->size = 0;

  T *_item = new T[degree - 1]();
  this->item = _item;

  Node<T> **_children = new Node<T> *[degree];
  for (int i = 0; i < degree; i++) {
    _children[i] = nullptr;
  }
  this->children = _children;

  this->parent = nullptr;
}

template <typename T> BPlusTree<T>::BPlusTree(int _degree) {
  this->root = nullptr;
  this->degree = _degree;
}

template <typename T> BPlusTree<T>::~BPlusTree() { clear(this->root); }

template <typename T> Node<T> *BPlusTree<T>::getRoot() { return this->root; }

template <typename T> Node<T> *BPlusTree<T>::_findKey(Node<T> *node, T key) {
  if (node == nullptr) {
    return nullptr;
  } else {
    Node<T> *cursor = node;

    while (!cursor->is_leaf) {
      for (int i = 0; i < cursor->size; i++) {
        if (key < cursor->item[i]) {

          cursor = cursor->children[i];
          break;
        }
        if (i == (cursor->size) - 1) {
          cursor = cursor->children[i + 1];
          break;
        }
      }
    }

    for (int i = 0; i < cursor->size; i++) {
      if (cursor->item[i] == key) {
        return cursor;
      }
    }

    return nullptr;
  }
}

template <typename T> Node<T> *_findRange(Node<T> *node, T key) {
  if (node == nullptr) {
    return nullptr;
  } else {
    Node<T> *cursor = node;

    while (!cursor->is_leaf) {
      for (int i = 0; i < cursor->size; i++) {
        if (key < cursor->item[i]) {

          cursor = cursor->children[i];
          break;
        }
        if (i == (cursor->size) - 1) {
          cursor = cursor->children[i + 1];
          break;
        }
      }
    }
    return cursor;
  }
}

template <typename T>
int BPlusTree<T>::searchRange(T start, T end, T *result_data, int arr_length) {
  int index = 0;

  Node<T> *start_node = _findRange(this->root, start);
  Node<T> *cursor = start_node;
  T temp = cursor->item[0];

  while (temp <= end) {
    if (cursor == nullptr) {
      break;
    }
    for (int i = 0; i < cursor->size; i++) {
      temp = cursor->item[i];
      if ((temp >= start) && (temp <= end)) {
        result_data[index] = temp;
        index++;
      }
    }
    cursor = cursor->children[cursor->size];
  }
  return index;
}

template <typename T> bool BPlusTree<T>::search(T data) {
  return _findKey(this->root, data) != nullptr;
}

template <typename T> int BPlusTree<T>::_findIndex(T *arr, T data, int len) {
  int index = 0;
  for (int i = 0; i < len; i++) {
    if (data < arr[i]) {
      index = i;
      break;
    }
    if (i == len - 1) {
      index = len;
      break;
    }
  }
  return index;
}

template <typename T> T *BPlusTree<T>::_insertItem(T *arr, T data, int len) {
  int index = 0;
  for (int i = 0; i < len; i++) {
    if (data < arr[i]) {
      index = i;
      break;
    }
    if (i == len - 1) {
      index = len;
      break;
    }
  }

  for (int i = len; i > index; i--) {
    arr[i] = arr[i - 1];
  }

  arr[index] = data;

  return arr;
}

template <typename T>
Node<T> **BPlusTree<T>::_innerInsert(Node<T> **child_arr, Node<T> *child,
                                     int len, int index) {
  for (int i = len; i > index; i--) {
    child_arr[i] = child_arr[i - 1];
  }
  child_arr[index] = child;
  return child_arr;
}

template <typename T>
Node<T> *BPlusTree<T>::_insertInChild(Node<T> *node, T data, Node<T> *child) {
  int item_index = 0;
  int child_index = 0;
  for (int i = 0; i < node->size; i++) {
    if (data < node->item[i]) {
      item_index = i;
      child_index = i + 1;
      break;
    }
    if (i == node->size - 1) {
      item_index = node->size;
      child_index = node->size + 1;
      break;
    }
  }
  for (int i = node->size; i > item_index; i--) {
    node->item[i] = node->item[i - 1];
  }
  for (int i = node->size + 1; i > child_index; i--) {
    node->children[i] = node->children[i - 1];
  }

  node->item[item_index] = data;
  node->children[child_index] = child;

  return node;
}

template <typename T>
void BPlusTree<T>::_insertInParent(Node<T> *par, Node<T> *child, T data) {

  Node<T> *cursor = par;
  if (cursor->size < this->degree - 1) {

    cursor = _insertInChild(cursor, data, child);
    cursor->size++;
  } else {

    auto *Newnode = new Node<T>(this->degree);
    Newnode->parent = cursor->parent;

    T *item_copy = new T[cursor->size + 1];
    for (int i = 0; i < cursor->size; i++) {
      item_copy[i] = cursor->item[i];
    }
    item_copy = _insertItem(item_copy, data, cursor->size);

    auto **child_copy = new Node<T> *[cursor->size + 2];
    for (int i = 0; i < cursor->size + 1; i++) {
      child_copy[i] = cursor->children[i];
    }
    child_copy[cursor->size + 1] = nullptr;
    child_copy = _innerInsert(child_copy, child, cursor->size + 1,
                              _findIndex(item_copy, data, cursor->size + 1));

    cursor->size = (this->degree) / 2;
    if ((this->degree) % 2 == 0) {
      Newnode->size = (this->degree) / 2 - 1;
    } else {
      Newnode->size = (this->degree) / 2;
    }

    for (int i = 0; i < cursor->size; i++) {
      cursor->item[i] = item_copy[i];
      cursor->children[i] = child_copy[i];
    }
    cursor->children[cursor->size] = child_copy[cursor->size];

    for (int i = 0; i < Newnode->size; i++) {
      Newnode->item[i] = item_copy[cursor->size + i + 1];
      Newnode->children[i] = child_copy[cursor->size + i + 1];
      Newnode->children[i]->parent = Newnode;
    }
    Newnode->children[Newnode->size] =
        child_copy[cursor->size + Newnode->size + 1];
    Newnode->children[Newnode->size]->parent = Newnode;

    T paritem = item_copy[this->degree / 2];

    delete[] item_copy;
    delete[] child_copy;

    if (cursor->parent == nullptr) {
      auto *Newparent = new Node<T>(this->degree);
      cursor->parent = Newparent;
      Newnode->parent = Newparent;

      Newparent->item[0] = paritem;
      Newparent->size++;

      Newparent->children[0] = cursor;
      Newparent->children[1] = Newnode;

      this->root = Newparent;

    } else {
      _insertInParent(cursor->parent, Newnode, paritem);
    }
  }
}

template <typename T> void BPlusTree<T>::insert(T data) {
  if (this->root == nullptr) {
    this->root = new Node<T>(this->degree);
    this->root->is_leaf = true;
    this->root->item[0] = data;
    this->root->size = 1;
  } else {
    Node<T> *cursor = this->root;

    cursor = _findRange(cursor, data);

    if (cursor->size < (this->degree - 1)) {

      cursor->item = _insertItem(cursor->item, data, cursor->size);
      cursor->size++;

      cursor->children[cursor->size] = cursor->children[cursor->size - 1];
      cursor->children[cursor->size - 1] = nullptr;
    } else {

      auto *Newnode = new Node<T>(this->degree);
      Newnode->is_leaf = true;
      Newnode->parent = cursor->parent;

      T *item_copy = new T[cursor->size + 1];
      for (int i = 0; i < cursor->size; i++) {
        item_copy[i] = cursor->item[i];
      }

      item_copy = _insertItem(item_copy, data, cursor->size);

      cursor->size = (this->degree) / 2;
      if ((this->degree) % 2 == 0) {
        Newnode->size = (this->degree) / 2;
      } else {
        Newnode->size = (this->degree) / 2 + 1;
      }

      for (int i = 0; i < cursor->size; i++) {
        cursor->item[i] = item_copy[i];
      }
      for (int i = 0; i < Newnode->size; i++) {
        Newnode->item[i] = item_copy[cursor->size + i];
      }

      cursor->children[cursor->size] = Newnode;
      Newnode->children[Newnode->size] = cursor->children[this->degree - 1];
      cursor->children[this->degree - 1] = nullptr;

      delete[] item_copy;

      T paritem = Newnode->item[0];

      if (cursor->parent == nullptr) {
        auto *Newparent = new Node<T>(this->degree);
        cursor->parent = Newparent;
        Newnode->parent = Newparent;

        Newparent->item[0] = paritem;
        Newparent->size++;

        Newparent->children[0] = cursor;
        Newparent->children[1] = Newnode;

        this->root = Newparent;
      } else {
        _insertInParent(cursor->parent, Newnode, paritem);
      }
    }
  }
}

template <typename T> void BPlusTree<T>::remove(T data) {

  Node<T> *cursor = this->root;

  cursor = _findRange(cursor, data);

  int sib_index = -1;
  for (int i = 0; i < cursor->parent->size + 1; i++) {
    if (cursor == cursor->parent->children[i]) {
      sib_index = i;
    }
  }
  int left = sib_index - 1;
  int right = sib_index + 1;

  int del_index = -1;
  for (int i = 0; i < cursor->size; i++) {
    if (cursor->item[i] == data) {
      del_index = i;
      break;
    }
  }

  if (del_index == -1) {
    return;
  }

  for (int i = del_index; i < cursor->size - 1; i++) {
    cursor->item[i] = cursor->item[i + 1];
  }
  cursor->item[cursor->size - 1] = 0;
  cursor->size--;

  if (cursor == this->root && cursor->size == 0) {
    clear(this->root);
    this->root = nullptr;
    return;
  }
  cursor->children[cursor->size] = cursor->children[cursor->size + 1];
  cursor->children[cursor->size + 1] = nullptr;

  if (cursor == this->root) {
    return;
  }
  if (cursor->size < degree / 2) {

    if (left >= 0) {
      Node<T> *leftsibling = cursor->parent->children[left];

      if (leftsibling->size > degree / 2) {
        T *temp = new T[cursor->size + 1];

        for (int i = 0; i < cursor->size; i++) {
          temp[i] = cursor->item[i];
        }

        _insertItem(temp, leftsibling->item[leftsibling->size - 1],
                    cursor->size);
        for (int i = 0; i < cursor->size + 1; i++) {
          cursor->item[i] = temp[i];
        }
        cursor->size++;
        delete[] temp;

        cursor->children[cursor->size] = cursor->children[cursor->size - 1];
        cursor->children[cursor->size - 1] = nullptr;

        leftsibling->item[leftsibling->size - 1] = 0;
        leftsibling->size--;
        leftsibling->children[leftsibling->size] =
            leftsibling->children[leftsibling->size + 1];
        leftsibling->children[leftsibling->size + 1] = nullptr;

        cursor->parent->item[left] = cursor->item[0];

        return;
      }
    }
    if (right <= cursor->parent->size) {
      Node<T> *rightsibling = cursor->parent->children[right];

      if (rightsibling->size > degree / 2) {
        T *temp = new T[cursor->size + 1];

        for (int i = 0; i < cursor->size; i++) {
          temp[i] = cursor->item[i];
        }

        _insertItem(temp, rightsibling->item[0], cursor->size);
        for (int i = 0; i < cursor->size + 1; i++) {
          cursor->item[i] = temp[i];
        }
        cursor->size++;
        delete[] temp;

        cursor->children[cursor->size] = cursor->children[cursor->size - 1];
        cursor->children[cursor->size - 1] = nullptr;

        for (int i = 0; i < rightsibling->size - 1; i++) {
          rightsibling->item[i] = rightsibling->item[i + 1];
        }
        rightsibling->item[rightsibling->size - 1] = 0;
        rightsibling->size--;
        rightsibling->children[rightsibling->size] =
            rightsibling->children[rightsibling->size + 1];
        rightsibling->children[rightsibling->size + 1] = nullptr;

        cursor->parent->item[right - 1] = rightsibling->item[0];

        return;
      }
    }

    if (left >= 0) {
      Node<T> *leftsibling = cursor->parent->children[left];

      for (int i = 0; i < cursor->size; i++) {
        leftsibling->item[leftsibling->size + i] = cursor->item[i];
      }

      leftsibling->children[leftsibling->size] = nullptr;
      leftsibling->size = leftsibling->size + cursor->size;
      leftsibling->children[leftsibling->size] = cursor->children[cursor->size];

      _removeFromParent(cursor, left, cursor->parent);
      for (int i = 0; i < cursor->size; i++) {
        cursor->item[i] = 0;
        cursor->children[i] = nullptr;
      }
      cursor->children[cursor->size] = nullptr;

      delete[] cursor->item;
      delete[] cursor->children;
      delete cursor;

      return;
    }
    if (right <= cursor->parent->size) {
      Node<T> *rightsibling = cursor->parent->children[right];

      for (int i = 0; i < rightsibling->size; i++) {
        cursor->item[i + cursor->size] = rightsibling->item[i];
      }

      cursor->children[cursor->size] = nullptr;
      cursor->size = rightsibling->size + cursor->size;
      cursor->children[cursor->size] =
          rightsibling->children[rightsibling->size];

      _removeFromParent(rightsibling, right - 1, cursor->parent);

      for (int i = 0; i < rightsibling->size; i++) {
        rightsibling->item[i] = 0;
        rightsibling->children[i] = nullptr;
      }
      rightsibling->children[rightsibling->size] = nullptr;

      delete[] rightsibling->item;
      delete[] rightsibling->children;
      delete rightsibling;
      return;
    }

  } else {
    return;
  }
}

template <typename T>
void BPlusTree<T>::_removeFromParent(Node<T> *node, int index, Node<T> *par) {
  Node<T> *remover = node;
  Node<T> *cursor = par;
  T target = cursor->item[index];

  if (cursor == this->root && cursor->size == 1) {
    if (remover == cursor->children[0]) {
      delete[] remover->item;
      delete[] remover->children;
      delete remover;
      this->root = cursor->children[1];
      delete[] cursor->item;
      delete[] cursor->children;
      delete cursor;
      return;
    }
    if (remover == cursor->children[1]) {
      delete[] remover->item;
      delete[] remover->children;
      delete remover;
      this->root = cursor->children[0];
      delete[] cursor->item;
      delete[] cursor->children;
      delete cursor;
      return;
    }
  }

  for (int i = index; i < cursor->size - 1; i++) {
    cursor->item[i] = cursor->item[i + 1];
  }
  cursor->item[cursor->size - 1] = 0;

  int rem_index = -1;
  for (int i = 0; i < cursor->size + 1; i++) {
    if (cursor->children[i] == remover) {
      rem_index = i;
    }
  }
  if (rem_index == -1) {
    return;
  }
  for (int i = rem_index; i < cursor->size; i++) {
    cursor->children[i] = cursor->children[i + 1];
  }
  cursor->children[cursor->size] = nullptr;
  cursor->size--;

  if (cursor == this->root) {
    return;
  }
  if (cursor->size < degree / 2) {

    int sib_index = -1;
    for (int i = 0; i < cursor->parent->size + 1; i++) {
      if (cursor == cursor->parent->children[i]) {
        sib_index = i;
      }
    }
    int left = sib_index - 1;
    int right = sib_index + 1;

    if (left >= 0) {
      Node<T> *leftsibling = cursor->parent->children[left];

      if (leftsibling->size > degree / 2) {
        T *temp = new T[cursor->size + 1];

        for (int i = 0; i < cursor->size; i++) {
          temp[i] = cursor->item[i];
        }

        _insertItem(temp, cursor->parent->item[left], cursor->size);
        for (int i = 0; i < cursor->size + 1; i++) {
          cursor->item[i] = temp[i];
        }
        cursor->parent->item[left] = leftsibling->item[leftsibling->size - 1];
        delete[] temp;

        Node<T> **child_temp = new Node<T> *[cursor->size + 2];

        for (int i = 0; i < cursor->size + 1; i++) {
          child_temp[i] = cursor->children[i];
        }

        _innerInsert(child_temp, leftsibling->children[leftsibling->size],
                     cursor->size, 0);

        for (int i = 0; i < cursor->size + 2; i++) {
          cursor->children[i] = child_temp[i];
        }
        delete[] child_temp;

        cursor->size++;
        leftsibling->size--;
        return;
      }
    }

    if (right <= cursor->parent->size) {
      Node<T> *rightsibling = cursor->parent->children[right];

      if (rightsibling->size > degree / 2) {
        T *temp = new T[cursor->size + 1];

        for (int i = 0; i < cursor->size; i++) {
          temp[i] = cursor->item[i];
        }

        _insertItem(temp, cursor->parent->item[sib_index], cursor->size);
        for (int i = 0; i < cursor->size + 1; i++) {
          cursor->item[i] = temp[i];
        }
        cursor->parent->item[sib_index] = rightsibling->item[0];
        delete[] temp;

        cursor->children[cursor->size + 1] = rightsibling->children[0];
        for (int i = 0; i < rightsibling->size; i++) {
          rightsibling->children[i] = rightsibling->children[i + 1];
        }
        rightsibling->children[rightsibling->size] = nullptr;

        cursor->size++;
        rightsibling->size--;
        return;
      }
    }

    if (left >= 0) {
      Node<T> *leftsibling = cursor->parent->children[left];

      leftsibling->item[leftsibling->size] = cursor->parent->item[left];

      for (int i = 0; i < cursor->size; i++) {
        leftsibling->item[leftsibling->size + i + 1] = cursor->item[i];
      }
      for (int i = 0; i < cursor->size + 1; i++) {
        leftsibling->children[leftsibling->size + i + 1] = cursor->children[i];
        cursor->children[i]->parent = leftsibling;
      }
      for (int i = 0; i < cursor->size + 1; i++) {
        cursor->children[i] = nullptr;
      }
      leftsibling->size = leftsibling->size + cursor->size + 1;

      _removeFromParent(cursor, left, cursor->parent);
      return;
    }
    if (right <= cursor->parent->size) {
      Node<T> *rightsibling = cursor->parent->children[right];

      cursor->item[cursor->size] = cursor->parent->item[right - 1];

      for (int i = 0; i < rightsibling->size; i++) {
        cursor->item[cursor->size + 1 + i] = rightsibling->item[i];
      }
      for (int i = 0; i < rightsibling->size + 1; i++) {
        cursor->children[cursor->size + i + 1] = rightsibling->children[i];
        rightsibling->children[i]->parent = rightsibling;
      }
      for (int i = 0; i < rightsibling->size + 1; i++) {
        rightsibling->children[i] = nullptr;
      }

      rightsibling->size = rightsibling->size + cursor->size + 1;

      _removeFromParent(rightsibling, right - 1, cursor->parent);
      return;
    }
  } else {
    return;
  }
}

template <typename T> void BPlusTree<T>::clear(Node<T> *cursor) {
  if (cursor != nullptr) {
    if (!cursor->is_leaf) {
      for (int i = 0; i <= cursor->size; i++) {
        clear(cursor->children[i]);
      }
    }
    delete[] cursor->item;
    delete[] cursor->children;
    delete cursor;
  }
}

template <typename T> void BPlusTree<T>::print() { _print(this->root); }

template <typename T> void BPlusTree<T>::_print(Node<T> *cursor) {

  if (cursor != NULL) {
    for (int i = 0; i < cursor->size; ++i) {
      std::cout << cursor->item[i] << " ";
    }
    std::cout << "\n";

    if (!cursor->is_leaf) {
      for (int i = 0; i < cursor->size + 1; ++i) {
        _print(cursor->children[i]);
      }
    }
  }
}
