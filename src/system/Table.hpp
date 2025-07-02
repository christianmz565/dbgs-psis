/**
 * @file Table.hpp
 * @brief Definición de la clase Table y estructuras auxiliares para la gestión de tablas de datos.
 *
 * Proporciona una estructura de tabla relacional con soporte para índices B+ y de índice invertido,
 * permitiendo operaciones de inserción, búsqueda e impresión de datos.
 */

#include "IndexEntry.hpp"
#include "../structures/BPlusTree.hpp"
#include "../structures/InvertedIndex.cpp"
#include <string>
#include <variant>
#include <vector>

/**
 * @enum DataType
 * @brief Tipos de datos soportados por las columnas de la tabla.
 */
enum class DataType { INT, DOUBLE, STRING };

/**
 * @typedef Cell
 * @brief Representa el valor de una celda, que puede ser int, double o string.
 */
using Cell = std::variant<int, double, std::string>;

/**
 * @struct Column
 * @brief Representa una columna de la tabla, con nombre y tipo de dato.
 */
struct Column {
  std::string name; /**< Nombre de la columna. */
  DataType type;    /**< Tipo de dato de la columna. */
};

/**
 * @class Table
 * @brief Representa una tabla de datos con soporte para índices y operaciones básicas.
 *
 * Permite definir un esquema de columnas, insertar filas, crear índices B+ e invertidos,
 * y realizar búsquedas eficientes por índice.
 */
class Table {
public:
  /**
   * @brief Constructor de Table.
   * @param cols Vector de pares (nombre, tipo) que define el esquema de la tabla.
   * @param hashCapacity Capacidad inicial para los mapas hash internos (por defecto 101).
   */
  Table(const std::vector<std::pair<std::string, DataType>> &cols,
        int hashCapacity = 101);

  /**
   * @brief Destructor de Table. Libera la memoria utilizada por los índices.
   */
  ~Table();

  /**
   * @brief Inserta una nueva fila en la tabla.
   * @param row Vector de celdas que representa la fila a insertar.
   */
  void insertRow(const std::vector<Cell> &row);

  /**
   * @brief Imprime el esquema (columnas y tipos) de la tabla.
   */
  void printSchema() const;

  /**
   * @brief Imprime todas las filas almacenadas en la tabla.
   */
  void printAllRows() const;

  /**
   * @brief Obtiene el valor de una celda dado el índice de fila y el nombre de columna.
   * @param rowIndex Índice de la fila.
   * @param colName Nombre de la columna.
   * @return Valor de la celda correspondiente.
   */
  Cell getCell(int rowIndex, const std::string &colName);

  /**
   * @brief Crea un índice B+ sobre una columna específica.
   * @param colName Nombre de la columna a indexar.
   * @param degree Grado del árbol B+.
   */
  void createBPlusTreeIndex(const std::string &colName, int degree);

  /**
   * @brief Crea un índice invertido sobre una columna específica.
   * @param colName Nombre de la columna a indexar.
   */
  void createInvertedIndex(const std::string &colName);

  /**
   * @brief Busca filas por valor de clave usando el índice correspondiente.
   * @param colName Nombre de la columna indexada.
   * @param key Valor de la clave a buscar.
   * @return Vector de identificadores de filas que coinciden con la clave.
   */
  std::vector<int> searchByIndex(const std::string &colName, const Cell &key);

private:
  std::vector<Column> schema_; /**< Esquema de columnas de la tabla. */
  HashMap<std::string, int> nameToIndex_; /**< Mapa de nombre de columna a índice. */
  std::vector<std::vector<Cell>> data_; /**< Datos almacenados en la tabla. */

  HashMap<std::string, BPlusTree<IndexEntry> *> bptIndices_; /**< Índices B+ por columna. */
  HashMap<std::string, InvertedIndex *> invIndices_;         /**< Índices invertidos por columna. */

  /**
   * @brief Verifica si una celda coincide con un tipo de dato.
   * @param cell Celda a verificar.
   * @param dt Tipo de dato esperado.
   * @return true si coincide, false en caso contrario.
   */
  static bool cellMatchesType(const Cell &cell, DataType dt);

  /**
   * @brief Convierte un tipo de dato a su representación en string.
   * @param dt Tipo de dato.
   * @return String representando el tipo de dato.
   */
  static std::string dataTypeToString(DataType dt);

  /**
   * @brief Obtiene el índice de una columna dado su nombre.
   * @param colName Nombre de la columna.
   * @return Índice de la columna en el esquema.
   */
  int columnIndex(const std::string &colName);
};
