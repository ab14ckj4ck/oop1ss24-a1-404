#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Player.hpp"
#include "CommandType.hpp"
#include "Phase.hpp"
#include "Map.hpp"
#include "FaultyIndex.hpp"
#include <algorithm>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

class Command
{
private:
  CommandType type_;
  std::vector<std::string> parameters_;

public:
  Command(std::vector<std::string>&  parameters);
  Command(CommandType type);
  Command(const Command& command) = default;
  
  ~Command() = default;

  bool isQuit();
  CommandType getType();
  std::vector<std::string>& getParameters();
  void setType(CommandType type);
  void printInfo(Player* curr_player);

  bool checkPlace(Command command, Map* game_map, Player* curr_player, Phase curr_phase);
  bool checkMove(Command command, Map* game_map, Player* curr_player, Phase curr_phase);
  bool isInt(Command command, FaultyIndex& faulty_index);
  bool isFloat(Command command, FaultyIndex& faulty_index);

  void placeCommand(Command command, Map* game_map, Player* curr_player);
  void moveCommand(Command command, Map* game_map, Player* curr_player);
  void passCommand(Player* curr_player);
};

#endif