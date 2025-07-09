#include "system/Parser.cpp"
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  Parser parser;

  parser.executeCommand("CREATE TABLE data | id:INT score:DOUBLE "
                        "review:STRING showId:STRING reviewId:STRING");

  std::string dataLocation;
  dataLocation = argc > 1 ? argv[1] : "";
  std::ifstream file(dataLocation.empty() ? "data.csv" : dataLocation);
  if (!file.is_open()) {
    std::cerr << "Error opening file data.csv\n";
    return 1;
  }

  std::string line;

  while (std::getline(file, line)) {
    std::vector<std::string> fields;
    std::string field;
    bool in_quotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
      char c = line[i];
      if (c == '"') {
        in_quotes = !in_quotes;
      } else if (c == ',' && !in_quotes) {
        fields.push_back(field);
        field.clear();
      } else {
        field += c;
      }
    }
    fields.push_back(field);

    std::string cmd = "INSERT INTO data | ";
    for (size_t i = 0; i < fields.size(); ++i) {
      std::string val = fields[i];
      val.erase(std::remove(val.begin(), val.end(), '\''), val.end());
      if (val.find(' ') != std::string::npos) {
        val = "'" + val + "'";
      }
      cmd += val;
      if (i + 1 < fields.size())
        cmd += " ";
    }

    std::cout << "\n> " << cmd << std::endl;
    parser.executeCommand(cmd);
  }
  parser.executeCommand("SELECT * FROM data");

  return 0;
}