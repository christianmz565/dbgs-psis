template <typename K, typename V> class HashNode {
public:
  V value;
  K key;

  HashNode(K key, V value);
};

template <typename K, typename V> class HashMap {
  HashNode<K, V> **arr;
  int capacity;
  int size;
  HashNode<K, V> *dummy;

public:
  HashMap(int cap);
  ~HashMap();

  int _hashCode(K key);

  void insert(K key, V value);
  V get(K key);
  V remove(K key);
  bool containsKey(K key);
  int getSize();
  bool isEmpty();
  void display();
  void clear();
};