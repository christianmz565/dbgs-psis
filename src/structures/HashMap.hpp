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
  HashNode<K, V> *dummy;
  long long *pows;

public:
  HashMap(int cap);
  ~HashMap();

  long long *_getPows();
  double _getLoadFactor();
  int _hashCode(K key);
  int _wrap(long long key, int size);
  void _insert(K key, V value, HashNode<K, V> **arr);
  void _grow();

  void insert(K key, V value);
  V get(K key);
  V remove(K key);
  bool containsKey(K key);
  int getSize();
  bool isEmpty();
  HashNode<K, V> **toList();
  void display();
  void clear();
};