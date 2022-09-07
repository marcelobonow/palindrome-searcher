#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char **argv)
{
  setlocale(LC_ALL, "");
  auto fileLocation = argc > 1 ? argv[1] : "data/pi.txt";
  auto index = argc > 2 ? std::atoi(argv[2]) : 0;
  auto digits = argc > 3 ? std::atoi(argv[3]) : 21;
  std::ifstream file;

  std::cout << "Procurando dígitos com offset de " << digits << "\n";

  file.open(fileLocation);
  file.seekg(index);
  char *buffer = (char *)malloc(sizeof(char) * digits);
  file.read(buffer, digits);
  std::cout << buffer;
}