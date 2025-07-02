#include "system/Table.cpp"
#include <iostream>

int main() {

  std::vector<std::pair<std::string, DataType>> cols = {
      {"id", DataType::INT},
      {"name", DataType::STRING},
      {"gpa", DataType::DOUBLE},
      {"comment", DataType::STRING}};

  Table students(cols);

  students.insertRow({1, std::string("Alice"), 3.9, std::string("CS major")});
  students.insertRow({2, std::string("Bob"), 3.4, std::string("Math minor")});
  students.insertRow({3, std::string("Carol"), 3.8, std::string("Physics")});
  students.insertRow({4, std::string("Dave"), 3.4, std::string("Math minor")});

  students.createBPlusTreeIndex("gpa", /*degree=*/3);

  students.createInvertedIndex("comment");

  students.printSchema();
  std::cout << "\nAll rows:\n";
  students.printAllRows();

  std::vector<int> matchesGPA = students.searchByIndex("gpa", Cell(3.4));
  std::cout << "\nRows with gpa == 3.4:\n";
  for (int rid : matchesGPA) {
    std::cout << "  Row #" << rid << " → ";

    std::string nm = std::get<std::string>(students.getCell(rid, "name"));
    std::cout << nm << "\n";
  }

  std::vector<int> matchesComment =
      students.searchByIndex("comment", Cell(std::string("Math minor")));
  std::cout << "\nRows whose comment == \"Math minor\":\n";
  for (int rid : matchesComment) {
    std::cout << "  Row #" << rid << " → ";
    std::string nm = std::get<std::string>(students.getCell(rid, "name"));
    std::cout << nm << "\n";
  }

  return 0;
}
