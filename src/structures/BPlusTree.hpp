template <typename T> struct Node {
  bool is_leaf;
  int degree;
  int size;
  T *item;
  Node<T> **children;
  Node<T> *parent;

public:
  Node(int _degree);
};

template <typename T> class BPlusTree {
  Node<T> *root;
  int degree;

public:
  BPlusTree(int _degree);
  ~BPlusTree();

  Node<T> *_findKey(Node<T> *node, T key);
  Node<T> *_findRange(Node<T> *node, T key);
  int _findIndex(T *arr, T data, int len);
  Node<T> **_innerInsert(Node<T> **child_arr, Node<T> *child, int len,
                         int index);
  T *_insertItem(T *arr, T data, int len);
  Node<T> *_insertInChild(Node<T> *node, T data, Node<T> *child);
  void _insertInParent(Node<T> *par, Node<T> *child, T data);
  void _removeFromParent(Node<T> *node, int index, Node<T> *par);
  void _print(Node<T> *cursor);

  Node<T> *getRoot();
  int searchRange(T start, T end, T *result_data, int arr_length);
  bool search(T data);
  void insert(T data);
  void remove(T data);
  void clear(Node<T> *cursor);
  void print();
};