/**
 * @file IndexEntry.hpp
 * @brief Definición de la estructura IndexEntry para índices de tablas.
 *
 * Contiene la clave (valor de celda) y el identificador de fila
 * correspondiente, junto con operadores de comparación.
 */

#include <string>
#include <variant>

using Cell = std::variant<int, double, std::string>;

/**
 * @struct IndexEntry
 * @brief Entrada de índice para estructuras de índice (B+ o invertido).
 *
 * Contiene la clave (valor de celda) y el identificador de fila
 * correspondiente.
 */
struct IndexEntry {
  Cell key;  /**< Clave del índice (valor de celda). */
  int rowId; /**< Identificador de la fila. */

  /**
   * @brief Constructor por defecto.
   * Crea una entrada de índice vacía con valores por defecto.
   */
  IndexEntry() : key(0), rowId(-1) {}

  /**
   * @brief Constructor con parámetros.
   * @param k Clave del índice
   * @param r Identificador de fila
   */
  IndexEntry(const Cell &k, int r) : key(k), rowId(r) {}

  /**
   * @brief Operador de comparación menor que.
   * Compara dos IndexEntry basándose en su clave.
   */
  bool operator<(const IndexEntry &other) const;
  /**
   * @brief Operador de comparación mayor que.
   * Compara dos IndexEntry basándose en su clave.
   */
  bool operator>(const IndexEntry &other) const;
  /**
   * @brief Operador de comparación menor o igual que.
   * Compara dos IndexEntry basándose en su clave.
   */
  bool operator<=(const IndexEntry &other) const;
  /**
   * @brief Operador de comparación mayor o igual que.
   * Compara dos IndexEntry basándose en su clave.
   */
  bool operator>=(const IndexEntry &other) const;
  /**
   * @brief Operador de comparación de igualdad.
   * Compara dos IndexEntry basándose en su clave y rowId.
   */
  bool operator==(const IndexEntry &other) const;
};