/**
 * @file InvertedIndex.hpp
 * @brief Implementación de un índice invertido utilizando un mapa hash.
 *
 * Esta clase permite asociar claves (palabras, términos, etc.) con listas de
 * identificadores de filas, facilitando búsquedas eficientes de ocurrencias de
 * términos en colecciones de datos.
 */

#include "HashMap.hpp"
#include <string>
#include <vector>

/**
 * @class InvertedIndex
 * @brief Índice invertido basado en HashMap para asociar claves con listas de
 * identificadores.
 *
 * Permite agregar, obtener, eliminar y limpiar asociaciones entre claves y
 * listas de enteros.
 */
class InvertedIndex {
public:
  /**
   * @brief Constructor de InvertedIndex.
   * @param initialCapacity Capacidad inicial del mapa hash subyacente (por
   * defecto 128).
   */
  InvertedIndex(int initialCapacity = 128);

  /**
   * @brief Destructor de InvertedIndex. Libera la memoria utilizada.
   */
  ~InvertedIndex();

  /**
   * @brief Agrega una asociación entre una clave y un identificador de fila.
   * @param key Clave a asociar.
   * @param rowId Identificador de fila a agregar.
   */
  void add(const std::string &key, int rowId);

  /**
   * @brief Obtiene la lista de identificadores asociados a una clave.
   * @param key Clave a buscar.
   * @return Vector de identificadores asociados. Si la clave no existe, retorna
   * un vector vacío.
   */
  std::vector<int> get(const std::string &key);

  /**
   * @brief Elimina la asociación de un identificador de fila para una clave
   * dada.
   * @param key Clave de la que se eliminará el identificador.
   * @param rowId Identificador de fila a eliminar.
   */
  void remove(const std::string &key, int rowId);

  /**
   * @brief Elimina todas las asociaciones del índice invertido.
   */
  void clear();

private:
  HashMap<std::string, std::vector<int>>
      map_; /**< Mapa hash que almacena las asociaciones clave-lista de
               identificadores. */
};
