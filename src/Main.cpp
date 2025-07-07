/** \file Main.cpp
 * Punto de entrada para la consola interactiva DBGS-PSIS.
 *
 * Este programa lanza una interfaz similar a un terminal que permite a los
 * usuarios ingresar y ejecutar comandos de base de datos de forma interactiva.
 * Escriba 'exit' o 'quit' para salir.
 */

#include "system/Parser.cpp"
#include <iostream>
#include <string>

/**
 * Punto de entrada principal del programa.
 *
 * Inicia la consola interactiva y permite a los usuarios ingresar comandos.
 * El programa continuará ejecutándose hasta que el usuario escriba 'exit' o
 * 'quit'.
 */
int main() {
  Parser parser;
  std::string cmd;
  std::cout
      << "Welcome to the DBGS-PSIS console. Type 'exit' or 'quit' to leave.\n";
  while (true) {
    std::cout << "> ";
    if (!std::getline(std::cin, cmd))
      break;
    if (cmd == "exit" || cmd == "quit")
      break;
    if (cmd.empty())
      continue;
    parser.executeCommand(cmd);
  }
  std::cout << "Goodbye!\n";
  return 0;
}
