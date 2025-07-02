/**
 * @file Parser.hpp
 * @brief Simple pipe syntax SQL parser for database operations.
 *
 * Provides a parser that can interpret pipe-separated SQL-like commands
 * and execute them on Table objects. Supports basic operations like
 * CREATE, INSERT, SELECT, DELETE, and INDEX operations.
 */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "Table.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

/**
 * @enum CommandType
 * @brief Types of SQL commands supported by the parser.
 */
enum class CommandType {
    CREATE_TABLE,
    INSERT,
    SELECT,
    DELETE,
    CREATE_INDEX,
    CREATE_INVERTED_INDEX,
    SHOW_SCHEMA,
    SHOW_TABLES,
    UNKNOWN
};

/**
 * @struct ParsedCommand
 * @brief Represents a parsed SQL command with its components.
 */
struct ParsedCommand {
    CommandType type;
    std::string tableName;
    std::vector<std::string> columns;
    std::vector<std::string> values;
    std::string whereColumn;
    std::string whereValue;
    std::string indexColumn;
    int indexDegree = 3; // Default B+ tree degree
};

/**
 * @class Parser
 * @brief Simple SQL parser with pipe syntax support.
 *
 * Parses and executes SQL-like commands using a pipe-separated syntax.
 * Example: "CREATE TABLE users | name:STRING age:INT | INSERT values"
 */
class Parser {
public:
    /**
     * @brief Constructor for Parser.
     */
    Parser();

    /**
     * @brief Destructor for Parser.
     */
    ~Parser();

    /**
     * @brief Parse and execute a SQL command string.
     * @param command The SQL command string to parse and execute.
     * @return True if command was executed successfully, false otherwise.
     */
    bool executeCommand(const std::string& command);

    /**
     * @brief Parse a command string into a ParsedCommand structure.
     * @param command The command string to parse.
     * @return ParsedCommand structure with parsed components.
     */
    ParsedCommand parseCommand(const std::string& command);

    /**
     * @brief Get a reference to a table by name.
     * @param tableName Name of the table to retrieve.
     * @return Pointer to the table, or nullptr if not found.
     */
    Table* getTable(const std::string& tableName);

    /**
     * @brief List all available tables.
     * @return Vector of table names.
     */
    std::vector<std::string> listTables() const;

private:
    std::unordered_map<std::string, std::unique_ptr<Table>> tables_;

    /**
     * @brief Execute a CREATE TABLE command.
     * @param cmd Parsed command structure.
     * @return True if successful, false otherwise.
     */
    bool executeCreateTable(const ParsedCommand& cmd);

    /**
     * @brief Execute an INSERT command.
     * @param cmd Parsed command structure.
     * @return True if successful, false otherwise.
     */
    bool executeInsert(const ParsedCommand& cmd);

    /**
     * @brief Execute a SELECT command.
     * @param cmd Parsed command structure.
     * @return True if successful, false otherwise.
     */
    bool executeSelect(const ParsedCommand& cmd);

    /**
     * @brief Execute a DELETE command.
     * @param cmd Parsed command structure.
     * @return True if successful, false otherwise.
     */
    bool executeDelete(const ParsedCommand& cmd);

    /**
     * @brief Execute a CREATE INDEX command.
     * @param cmd Parsed command structure.
     * @return True if successful, false otherwise.
     */
    bool executeCreateIndex(const ParsedCommand& cmd);

    /**
     * @brief Execute a CREATE INVERTED INDEX command.
     * @param cmd Parsed command structure.
     * @return True if successful, false otherwise.
     */
    bool executeCreateInvertedIndex(const ParsedCommand& cmd);

    /**
     * @brief Execute a SHOW SCHEMA command.
     * @param cmd Parsed command structure.
     * @return True if successful, false otherwise.
     */
    bool executeShowSchema(const ParsedCommand& cmd);

    /**
     * @brief Execute a SHOW TABLES command.
     * @return True if successful, false otherwise.
     */
    bool executeShowTables();

    /**
     * @brief Parse data type string to DataType enum.
     * @param typeStr String representation of the data type.
     * @return DataType enum value.
     */
    DataType parseDataType(const std::string& typeStr);

    /**
     * @brief Parse a value string to appropriate Cell type.
     * @param valueStr String representation of the value.
     * @param dataType Expected data type.
     * @return Cell containing the parsed value.
     */
    Cell parseValue(const std::string& valueStr, DataType dataType);

    /**
     * @brief Split a string by delimiter.
     * @param str String to split.
     * @param delimiter Character to split by.
     * @return Vector of split strings.
     */
    std::vector<std::string> split(const std::string& str, char delimiter);

    /**
     * @brief Trim whitespace from a string.
     * @param str String to trim.
     * @return Trimmed string.
     */
    std::string trim(const std::string& str);

    /**
     * @brief Convert string to uppercase.
     * @param str String to convert.
     * @return Uppercase string.
     */
    std::string toUpper(const std::string& str);
};

#endif // PARSER_HPP