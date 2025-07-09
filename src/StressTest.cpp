#include "system/Parser.cpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
  Parser parser;

  int testSize = argc > 1 ? std::stoi(argv[1]) : 1000;
  std::cerr << "Running stress test with " << testSize
            << " insertions...\n";

  parser.executeCommand(
      "CREATE TABLE students | id:INT name:STRING gpa:DOUBLE comment:STRING");
  for (int i = 0; i < testSize; ++i)
  {
    std::string cmd = "INSERT INTO students | " + std::to_string(i) + " Student" +
                      std::to_string(i) + " 3.0 'Test comment'";
    parser.executeCommand(cmd);
  }

  return 0;
}