#include "riot_games.h"
#include <iostream>
#include <algorithm>

int main(int argc, char** argv) {
  std::string input;
  std::string local;
  while(std::getline(std::cin, local)) {
    input += local;
  }
  umison::champion fea;
  if(!fea.read_data(input)) {
    std::cout << "Failed\n";
  } else {
    std::cout << "Passed\n";
  }
}