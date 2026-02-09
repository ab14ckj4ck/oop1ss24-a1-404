#ifndef MAP_HPP
#define MAP_HPP

#include "Player.hpp"
#include "Field.hpp"
#include <fstream>
#include <string>
#include <vector>

class Map {
private:
  int columns_;
  int rows_;
  std::vector<std::vector<Field*>> fields_;
  bool output_active_;

public:
  Map(char* config_path, Player* player_a, Player* player_b);
  Map(Map& map) = delete;
  ~Map();

  void loadConfig(char* config_path, Player* player_a, Player* player_b);
  void printMap();
  void setOutputActive();
  Field* getField(int x, int y);
  int getColumns();
  int getRows();
};

#endif