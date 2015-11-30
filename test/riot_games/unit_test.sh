#!/bin/bash

../umison -i riot_games.xml -e riot_games.h -p riot_games.cpp

g++ -g -I../../rapidjson/include -o riot_games main.cpp riot_games.cpp

./riot_games

AFL_HARDEN=1 afl-g++ -O3 -fPIC -I../../rapidjson/include -o riot_games_afl afl_main.cpp riot_games.cpp
