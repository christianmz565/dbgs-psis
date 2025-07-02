/**
 * @file HashMap.hpp
 * @brief Implementación de un mapa hash genérico con manejo de colisiones y
 * redimensionamiento automático.
 *
 * Este archivo define las clases HashNode y HashMap, que permiten almacenar
 * pares clave-valor de tipo genérico, con operaciones de inserción, búsqueda,
 * eliminación y utilidades adicionales.
 */

#include <iostream>
#include <optional>
#include <sstream>
#include <string>

/**
 * @class HashNode
 * @brief Nodo que almacena un par clave-valor en la tabla hash.
 *
 * @tparam K Tipo de la clave.
 * @tparam V Tipo del valor.
 */
template <typename K, typename V> class HashNode {
public:
  V value; /**< Valor almacenado en el nodo. */
  K key;   /**< Clave asociada al valor. */

  /**
   * @brief Constructor de HashNode.
   * @param key Clave del nodo.
   * @param value Valor asociado a la clave.
   */
  HashNode(K key, V value);
};

/**
 * @class HashMap
 * @brief Implementación genérica de un mapa hash con direccionamiento abierto y
 * redimensionamiento.
 *
 * Permite almacenar, buscar, eliminar y mostrar pares clave-valor. Utiliza
 * direccionamiento abierto con sondeo cuadrático para resolver colisiones y
 * redimensiona automáticamente la tabla cuando la carga supera cierto umbral.
 *
 * @tparam K Tipo de la clave.
 * @tparam V Tipo del valor.
 */
template <typename K, typename V> class HashMap {
  HashNode<K, V> **table; /**< Tabla hash de punteros a nodos. */
  int capacity;           /**< Capacidad actual de la tabla. */
  int size;               /**< Número de elementos almacenados. */
  long long *pows;        /**< Potencias precomputadas para el hash. */

public:
  /**
   * @brief Constructor de HashMap.
   * @param cap Capacidad inicial de la tabla hash.
   * @note La capacidad debe ser un número potencia de 2 para evitar escenarios
   * donde el redireccionamiento no encuentre un espacio libre. Se recomienda
   * usar un valor mayor a 16 para evitar colisiones frecuentes.
   * @tparam K Tipo de la clave.
   * @tparam V Tipo del valor.
   */
  HashMap(int cap);

  /**
   * @brief Destructor de HashMap. Libera la memoria utilizada.
   */
  ~HashMap();

  /**
   * @brief Calcula y retorna el arreglo de potencias para el hash.
   * @return Puntero al arreglo de potencias.
   */
  long long *_getPows();

  /**
   * @brief Calcula el factor de carga actual de la tabla.
   * @return Factor de carga (size/capacity).
   */
  double _getLoadFactor();

  /**
   * @brief Convierte un valor genérico a string para el hash.
   * @param val Valor a convertir.
   * @return Representación en string del valor.
   */
  std::string _toStringGeneric(const K &val);

  /**
   * @brief Calcula el código hash para una clave.
   * @param key Clave a hashear.
   * @return Índice hash calculado.
   */
  int _hashCode(K key);

  /**
   * @brief Ajusta un valor hash al rango de la tabla.
   * @param key Valor hash.
   * @param size Tamaño de la tabla.
   * @return Valor ajustado al rango [0, size).
   */
  int _wrap(long long key, int size);

  /**
   * @brief Inserta un par clave-valor en un arreglo de nodos.
   * @param key Clave a insertar.
   * @param value Valor a insertar.
   * @param arr Arreglo de nodos donde insertar.
   */
  void _insert(K key, V value, HashNode<K, V> **arr);

  /**
   * @brief Duplica la capacidad de la tabla y reubica los elementos.
   */
  void _grow();

  /**
   * @brief Inserta un par clave-valor en el mapa.
   * @param key Clave a insertar.
   * @param value Valor a insertar.
   */
  void insert(K key, V value);

  /**
   * @brief Obtiene el valor asociado a una clave.
   * @param key Clave a buscar.
   * @return Valor asociado si existe, std::nullopt en caso contrario.
   */
  std::optional<V> get(K key);

  /**
   * @brief Elimina un par clave-valor del mapa.
   * @param key Clave a eliminar.
   * @return Valor eliminado si existía, std::nullopt en caso contrario.
   */
  std::optional<V> remove(K key);

  /**
   * @brief Verifica si una clave existe en el mapa.
   * @param key Clave a buscar.
   * @return true si existe, false en caso contrario.
   */
  bool containsKey(K key);

  /**
   * @brief Retorna el número de elementos almacenados.
   * @return Número de elementos.
   */
  int getSize();

  /**
   * @brief Verifica si el mapa está vacío.
   * @return true si está vacío, false en caso contrario.
   */
  bool isEmpty();

  /**
   * @brief Retorna un arreglo con todos los nodos almacenados.
   * @return Puntero al arreglo de nodos.
   */
  HashNode<K, V> **toList();

  /**
   * @brief Muestra por consola todos los pares clave-valor almacenados.
   */
  void display();

  /**
   * @brief Elimina todos los elementos del mapa y libera la memoria.
   */
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
