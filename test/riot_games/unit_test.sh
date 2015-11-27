#!/bin/bash

../cudason -i riot_games.xml -e riot_games.h -p riot_games.cpp

g++ -g -I../../rapidjson/include -o riot_games main.cpp riot_games.cpp

./riot_games

afl-g++ -g --disable-shared -fsanitize=address -I../rapidjson/include -o riot_games_afl main.cpp riot_games.cpp
