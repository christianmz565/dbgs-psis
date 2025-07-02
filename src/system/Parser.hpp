/**
 * @file Parser.hpp
 * @brief Analizador SQL simple con sintaxis de pipas para operaciones de base
 * de datos.
 *
 * Proporciona un analizador que puede interpretar comandos tipo SQL separados
 * por pipas y ejecutarlos sobre objetos Table. Soporta operaciones básicas como
 * CREATE, INSERT, SELECT, DELETE e índices.
 */

#include "Table.cpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @enum CommandType
 * @brief Tipos de comandos SQL soportados por el analizador.
 */
enum class CommandType {
  CREATE_TABLE,          /**< Crear tabla */
  INSERT,                /**< Insertar datos */
  SELECT,                /**< Seleccionar datos */
  DELETE,                /**< Eliminar datos */
  CREATE_INDEX,          /**< Crear índice B+ */
  CREATE_INVERTED_INDEX, /**< Crear índice invertido */
  SHOW_SCHEMA,           /**< Mostrar esquema de tabla */
  SHOW_TABLES,           /**< Mostrar todas las tablas */
  UNKNOWN                /**< Comando desconocido */
};

/**
 * @struct ParsedCommand
 * @brief Representa un comando SQL analizado con sus componentes.
 */
struct ParsedCommand {
  CommandType type;                 /**< Tipo de comando */
  std::string tableName;            /**< Nombre de la tabla */
  std::vector<std::string> columns; /**< Columnas involucradas */
  std::vector<std::string> values;  /**< Valores a insertar o comparar */
  std::string whereColumn;          /**< Columna para condición WHERE */
  std::string whereValue;           /**< Valor para condición WHERE */
  std::string indexColumn;          /**< Columna para índice */
  int indexDegree = 3;              /**< Grado por defecto del B+ tree */
};

/**
 * @class Parser
 * @brief Analizador SQL simple con soporte de sintaxis de tubería.
 *
 * Analiza y ejecuta comandos tipo SQL usando una sintaxis separada por pipas.
 * Ejemplo: "CREATE TABLE users | name:STRING age:INT | INSERT values"
 */
class Parser {
public:
  /**
   * @brief Constructor de Parser.
   */
  Parser();

  /**
   * @brief Destructor de Parser.
   */
  ~Parser();

  /**
   * @brief Analiza y ejecuta un comando SQL.
   * @param command Cadena del comando SQL a analizar y ejecutar.
   * @return true si el comando se ejecutó correctamente, false en caso
   * contrario.
   */
  bool executeCommand(const std::string &command);

  /**
   * @brief Analiza una cadena de comando en una estructura ParsedCommand.
   * @param command Cadena del comando a analizar.
   * @return Estructura ParsedCommand con los componentes analizados.
   */
  ParsedCommand parseCommand(const std::string &command);

  /**
   * @brief Obtiene una referencia a una tabla por nombre.
   * @param tableName Nombre de la tabla a recuperar.
   * @return Puntero a la tabla, o nullptr si no se encuentra.
   */
  Table *getTable(const std::string &tableName);

  /**
   * @brief Lista todas las tablas disponibles.
   * @return Vector con los nombres de las tablas.
   */
  std::vector<std::string> listTables() const;

private:
  std::unordered_map<std::string, std::unique_ptr<Table>> tables_;

  /**
   * @brief Ejecuta un comando CREATE TABLE.
   * @param cmd Estructura de comando analizado.
   * @return true si fue exitoso, false en caso contrario.
   */
  bool executeCreateTable(const ParsedCommand &cmd);

  /**
   * @brief Ejecuta un comando INSERT.
   * @param cmd Estructura de comando analizado.
   * @return true si fue exitoso, false en caso contrario.
   */
  bool executeInsert(const ParsedCommand &cmd);

  /**
   * @brief Ejecuta un comando SELECT.
   * @param cmd Estructura de comando analizado.
   * @return true si fue exitoso, false en caso contrario.
   */
  bool executeSelect(const ParsedCommand &cmd);

  /**
   * @brief Ejecuta un comando DELETE.
   * @param cmd Estructura de comando analizado.
   * @return true si fue exitoso, false en caso contrario.
   */
  bool executeDelete(const ParsedCommand &cmd);

  /**
   * @brief Ejecuta un comando CREATE INDEX.
   * @param cmd Estructura de comando analizado.
   * @return true si fue exitoso, false en caso contrario.
   */
  bool executeCreateIndex(const ParsedCommand &cmd);

  /**
   * @brief Ejecuta un comando CREATE INVERTED INDEX.
   * @param cmd Estructura de comando analizado.
   * @return true si fue exitoso, false en caso contrario.
   */
  bool executeCreateInvertedIndex(const ParsedCommand &cmd);

  /**
   * @brief Ejecuta un comando SHOW SCHEMA.
   * @param cmd Estructura de comando analizado.
   * @return true si fue exitoso, false en caso contrario.
   */
  bool executeShowSchema(const ParsedCommand &cmd);

  /**
   * @brief Ejecuta un comando SHOW TABLES.
   * @return true si fue exitoso, false en caso contrario.
   */
  bool executeShowTables();

  /**
   * @brief Parsea una cadena de tipo de dato a DataType.
   * @param typeStr Cadena con la representación del tipo de dato.
   * @return Valor del enum DataType.
   */
  DataType parseDataType(const std::string &typeStr);

  /**
   * @brief Parsea una cadena de valor al tipo Cell correspondiente.
   * @param valueStr Cadena con el valor.
   * @param dataType Tipo de dato esperado.
   * @return Cell con el valor parseado.
   */
  Cell parseValue(const std::string &valueStr, DataType dataType);

  /**
   * @brief Parsea un valor de cadena a un Cell según el tipo de dato.
   * @param type Tipo de dato destino.
   * @param value Valor de cadena a convertir.
   * @return Cell con el valor convertido al tipo adecuado.
   */
  Cell parseCellByType(DataType type, const std::string &value);

  /**
   * @brief Divide una cadena por un delimitador.
   * @param str Cadena a dividir.
   * @param delimiter Carácter delimitador.
   * @param escapeChar Carácter de escape.
   * @return Vector de cadenas resultantes.
   */
  std::vector<std::string> split(const std::string &str, char delimiter,
                                 char escapeChar = '\0');

  /**
   * @brief Elimina espacios en blanco de una cadena.
   * @param str Cadena a limpiar.
   * @return Cadena sin espacios en blanco.
   */
  std::string trim(const std::string &str);

  /**
   * @brief Convierte una cadena a mayúsculas.
   * @param str Cadena a convertir.
   * @return Cadena en mayúsculas.
   */
  std::string toUpper(const std::string &str);
};
