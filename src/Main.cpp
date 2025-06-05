#include "system/Table.hpp"

int main() {

  std::vector<std::pair<std::string, DataType>> cols = {
      {"id", DataType::INT},
      {"name", DataType::STRING},
      {"gpa", DataType::DOUBLE}};
  Table students(cols);

  students.printSchema();
  std::cout << "\n";

  students.insertRow({1, std::string("Alice"), 3.8});

  students.insertRow({{"name", std::string("Bob")}, {"id", 2}, {"gpa", 3.4}});

  students.printAllRows();
  std::cout << "\n";

  Cell c = students.getCell(1, "name");
  std::string bobName = std::get<std::string>(c);
  std::cout << "Row 2, column 'name' = " << bobName << "\n";

  return 0;
}