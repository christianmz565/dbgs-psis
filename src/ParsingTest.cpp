#include "system/Parser.cpp"
#include <iostream>
#include <string>
#include <vector>

int main() {
  Parser parser;
  std::vector<std::string> commands = {
      // CREATE TABLE
      "CREATE TABLE students | id:INT name:STRING gpa:DOUBLE comment:STRING",
      // INSERT rows
      "INSERT INTO students | 1 Alice 3.9 'CS major'",
      "INSERT INTO students | 2 Bob 3.4 'Math minor'",
      "INSERT INTO students | 3 Carol 3.8 Physics",
      "INSERT INTO students | 4 Dave 3.7 'Math minor'",
      // SHOW TABLES
      "SHOW TABLES",
      // SHOW SCHEMA
      "SHOW SCHEMA students",
      // SELECT all
      "SELECT * FROM students",
      // CREATE B+ TREE INDEX
      "CREATE INDEX ON students | gpa 3",
      // CREATE INVERTED INDEX
      "CREATE INVERTED INDEX ON students | comment",
      // SELECT with WHERE (B+ Tree index)
      "SELECT * FROM students | WHERE gpa = 3.4",
      // SELECT with WHERE (Inverted index)
      "SELECT * FROM students | WHERE comment = 'Math minor'",
      // SELECT with WHERE (no index)
      "SELECT * FROM students | WHERE name = Bob",
      // DELETE with WHERE
      "DELETE FROM students | WHERE name = Bob",
      // SELECT after DELETE
      "SELECT * FROM students",
  };

  for (const auto &cmd : commands) {
    std::cout << "\n> " << cmd << std::endl;
    parser.executeCommand(cmd);
  }

  return 0;
}
