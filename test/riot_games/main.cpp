#include "riot_games.h"
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
  // TEST 1
  {
    std::cout << "[TEST 1] ";
    std::string input_text;
    std::ifstream fi("test_champion.json");
    if(fi) {
      char buf[1024];
      while(fi.getline(buf,1024)){
        input_text += buf;
      }
      cudason::champion champ;
      if(!champ.read_data(input_text)) {
        std::cout << "Failed\n";
      } else {
        std::cout << "Passed\n";
      }
    } else {
      std::cout << "Failed\n";
    }
  }
  // TEST 2
  {
    std::cout << "[TEST 2] ";
    std::string input_text;
    std::ifstream fi("test_get_spectator_game_info.json");
    if(fi) {
      char buf[1024];
      while(fi.getline(buf,1024)){
        input_text += buf;
      }
      cudason::getSpectatorGameInfo spect;
      if(!spect.read_data(input_text)) {
        std::cout << "Failed\n";
      } else {
        std::cout << "Passed\n";
      }
    } else {
      std::cout << "Failed\n";
    }
  }
  // TEST 3
  {
    std::cout << "[TEST 3] ";
    std::string input_text;
    std::ifstream fi("test_featured_games.json");
    if(fi) {
      char buf[1024];
      while(fi.getline(buf,1024)){
        input_text += buf;
      }
      cudason::featuredGames fea;
      if(!fea.read_data(input_text)) {
        std::cout << "Failed\n";
      } else {
        std::cout << "Passed\n";
      }
    } else {
      std::cout << "Failed\n";
    }
  }
  // TEST 4
  {
    std::cout << "[TEST 4] ";
    std::string input_text;
    std::ifstream fi("test_recent_game_by_summoner_ids.json");
    if(fi) {
      char buf[1024];
      while(fi.getline(buf,1024)){
        input_text += buf;
      }
      cudason::recent_game_by_summoner_id fea;
      if(!fea.read_data(input_text)) {
        std::cout << "Failed\n";
      } else {
        std::cout << "Passed\n";
      }
    } else {
      std::cout << "Failed\n";
    }
  }
  // TEST 5
  {
    std::cout << "[TEST 5] ";
    std::string input_text;
    std::ifstream fi("test_get_leagues_by_symmoner_ids.json");
    if(fi) {
      char buf[1024];
      while(fi.getline(buf,1024)){
        input_text += buf;
      }
      cudason::get_leagues_by_summoner_ids fea;
      if(!fea.read_data(input_text)) {
        std::cout << "Failed\n";
      } else {
        std::cout << "Passed\n";
      }
    } else {
      std::cout << "Failed\n";
    }
  }
}
