#include "Command.hpp"

Command::Command(std::vector<std::string>& input) : parameters_(input) {
  std::string command = parameters_[0];
  std::transform(command.begin(), command.end(), command.begin(), ::tolower);
  parameters_.erase(parameters_.begin());
  if (command == "quit") {
    type_ = CommandType::QUIT;
  }
  else if (command == "place") {
    type_ = CommandType::PLACE;
  }
  else if (command == "move") {
    type_ = CommandType::MOVE;
  }
  else if (command == "pass") {
    type_ = CommandType::PASS;
  }
  else if (command == "map") {
    type_ = CommandType::MAP;
  }
  else if (command == "info") {
    type_ = CommandType::INFO;
  }
  else {
    type_ = CommandType::INVALID;
  }
}

Command::Command(CommandType type) : type_(type) {}

bool Command::isQuit() {
  if (type_ == CommandType::QUIT) {
    return true;
  }
  return false;
}

CommandType Command::getType() {
  return type_;
}

void Command::setType(CommandType type) {
  type_ = type;
}

std::vector<std::string>& Command::getParameters() {
  return parameters_;
}

void Command::printInfo(Player* curr_player) {
  std::cout << "Player " << curr_player->getId() << ":" << std::endl;
  std::cout << "You have " << curr_player->getChips() << " chip(s) saved!" << std::endl;
  std::cout << "You own " << curr_player->getClaimedFields() << " field(s)!\n" << std::endl;
}

bool Command::checkPlace(Command command, Map* game_map, Player* curr_player, Phase curr_phase) {
  if (curr_phase != Phase::PLACEMENT) {
    std::cout << "[ERROR] Entered command is not valid in this phase!" << std::endl;
    return false;
  }

  FaultyIndex faulty_index = FaultyIndex::CORRECT;
  if (!isInt(command, faulty_index)) {
    if (faulty_index == FaultyIndex::AMOUNT) {
      std::cout << "[ERROR] Invalid amount! Must be a number > 0!" << std::endl;
    }
    else if (faulty_index == FaultyIndex::OLD_X || faulty_index == FaultyIndex::OLD_Y) {
      std::cout << "[ERROR] Invalid field!" << std::endl;
    }
    return false;
  }

  if (std::stoi(command.getParameters()[0]) > curr_player->getChips()) {
    std::cout << "[ERROR] Invalid amount! Must be a number <= chips in player inventory!" << std::endl;
    return false;
  }
  if (std::stoi(command.getParameters()[0]) <= 0) {
    std::cout << "[ERROR] Invalid amount! Must be a number > 0!" << std::endl;
    return false;
  }
  int new_x = std::stoi(command.getParameters()[1]);
  int new_y = std::stoi(command.getParameters()[2]);

  if (new_x < 1 || new_y < 1) {
    std::cout << "[ERROR] Invalid field!" << std::endl;
    return false;
  }

  new_x -= 1;
  new_y -= 1;

  if (new_x >= game_map->getColumns() || new_y >= game_map->getRows() || new_x < 0 || new_y < 0) {
    std::cout << "[ERROR] Invalid field!" << std::endl;
    return false;
  }

  Field* new_field = game_map->getField(new_x, new_y);

  if (new_field->getPlayer() != curr_player || new_x >= game_map->getColumns() || new_y >= game_map->getRows() ||
      new_x < 0 || new_y < 0 || new_field->isWater()) {
    std::cout << "[ERROR] Invalid field!" << std::endl;
    return false;
  }
  return true;
}

void Command::placeCommand(Command command, Map* game_map, Player* curr_player) {
  int place_chips = std::stoi(command.getParameters()[0]);
  int new_x = std::stoi(command.getParameters()[1]) - 1;
  int new_y = std::stoi(command.getParameters()[2]) - 1;

  Field* new_field = game_map->getField(new_x, new_y);
  new_field->addChips(place_chips);
  new_field->setPlayer(curr_player);
  curr_player->removeChips(place_chips);
}

bool Command::checkMove(Command command, Map* game_map, Player* curr_player, Phase curr_phase) {
  if (curr_phase != Phase::MOVEMENT) {
    std::cout << "[ERROR] Entered command is not valid in this phase!" << std::endl;
    return false;
  }

  FaultyIndex faulty_index = FaultyIndex::CORRECT;
  if (!isInt(command, faulty_index)) {
    if (faulty_index == FaultyIndex::AMOUNT) {
      std::cout << "[ERROR] Invalid amount! Must be a number > 0!" << std::endl;
    }
    else if (faulty_index == FaultyIndex::OLD_X || faulty_index == FaultyIndex::OLD_Y) { 
      std::cout << "[ERROR] Invalid origin!" << std::endl;
    }
    else if (faulty_index == FaultyIndex::NEW_X || faulty_index == FaultyIndex::NEW_Y) {
      std::cout << "[ERROR] Invalid destination!" << std::endl;
    }
    return false;
  }
  if (std::stoi(command.getParameters()[0]) <= 0) {
    std::cout << "[ERROR] Invalid amount! Must be a number > 0!" << std::endl;
    return false;
  }

  int old_x = std::stoi(command.getParameters()[1]);
  int old_y = std::stoi(command.getParameters()[2]);
  int new_x = std::stoi(command.getParameters()[3]);
  int new_y = std::stoi(command.getParameters()[4]);

  if (old_x < 1 || old_y < 1) {
    std::cout << "[ERROR] Invalid origin!" << std::endl;
    return false;
  }

  if (new_x < 1 || new_y < 1) {
    std::cout << "[ERROR] Invalid destination!" << std::endl;
    return false;
  }

  old_x -= 1;
  old_y -= 1;
  new_x -= 1;
  new_y -= 1;

  if (old_y >= game_map->getRows() || old_x >= game_map->getColumns() || old_x < 0 || old_y < 0) {
    std::cout << "[ERROR] Invalid origin!" << std::endl;
    return false;
  }

  Field* old_field = game_map->getField(old_x, old_y);

  if (old_field->getPlayer() != curr_player) {
    std::cout << "[ERROR] Invalid origin!" << std::endl;
    return false;
  }

  if (new_y >= game_map->getRows() || new_x >= game_map->getColumns() || new_x < 0 || new_y < 0) {
    std::cout << "[ERROR] Invalid destination!" << std::endl;
    return false;
  }

  Field* new_field = game_map->getField(new_x, new_y);

  if (std::stoi(command.getParameters()[0]) > old_field->getChips()) {
    std::cout << "[ERROR] Invalid amount! Must be a number <= chips on origin field!" << std::endl;
    return false;
  }

  if (old_field == new_field || new_field->isWater()) {
    std::cout << "[ERROR] Invalid destination!" << std::endl;
    return false;
  }

  int diff_x = std::abs(new_x - old_x);
  int diff_y = std::abs(new_y - old_y);

  if (diff_x > 1 || diff_y > 1 || (diff_x + diff_y) > 2) {
    std::cout << "[ERROR] Invalid destination!" << std::endl;
    return false;
  }
  return true;
}

void Command::moveCommand(Command command, Map* game_map, Player* curr_player) {
  int move_chips = std::stoi(command.getParameters()[0]);
  int old_x = std::stoi(command.getParameters()[1]) - 1;
  int old_y = std::stoi(command.getParameters()[2]) - 1;
  int new_x = std::stoi(command.getParameters()[3]) - 1;
  int new_y = std::stoi(command.getParameters()[4]) - 1;

  Field* old_field = game_map->getField(old_x, old_y);
  Field* new_field = game_map->getField(new_x, new_y);

  if (new_field->getPlayer() == curr_player || new_field->getPlayer() == nullptr) {
    if (new_field->getPlayer() == nullptr) {
      curr_player->addClaimedFields(1);
    }
    new_field->addChips(move_chips);
    new_field->setPlayer(curr_player);
    old_field->setChips(old_field->getChips() - move_chips);

    if (old_field->getChips() <= 0) {
      old_field->setChips(0);
      old_field->setPlayer(nullptr);
      curr_player->removeClaimedFields(1);
    }
    return;
  }
  if (new_field->getPlayer() != curr_player) {
    int result = (new_field->getChips() + 1) - move_chips;
    if (result < 0) {
      new_field->getPlayer()->removeClaimedFields(1);
      new_field->setPlayer(curr_player);
      new_field->setChips(std::abs(result));
      curr_player->addClaimedFields(1);
    }
    else if (result > 0) {
      new_field->setChips(result);
    }
    else if (result == 0) {
      new_field->getPlayer()->removeClaimedFields(1);
      new_field->setChips(0);
      new_field->setPlayer(nullptr);
    }
    if ((old_field->getChips() - move_chips) == 0) {
      old_field->setChips(0);
      old_field->setPlayer(nullptr);
      curr_player->removeClaimedFields(1);
    }
    return;
  }
}

bool Command::isInt(Command command, FaultyIndex& faulty_index) {
  if (!isFloat(command, faulty_index)) {
    return false;
  }
  for (std::size_t i = 0; i < command.getParameters().size(); i++) {
    try {
      std::stoi(command.getParameters()[i]);
    }
    catch (const std::invalid_argument& e) {
      if (i == 0) {
        faulty_index = FaultyIndex::AMOUNT;
      }
      else if (i == 1) {
        faulty_index = FaultyIndex::OLD_X;
      }
      else if (i == 2) {
        faulty_index = FaultyIndex::OLD_Y;
      }
      else if (i == 3) {
        faulty_index = FaultyIndex::NEW_X;
      }
      else if (i == 4) {
        faulty_index = FaultyIndex::NEW_Y;
      }
      return false;
    }
    catch (const std::out_of_range& e) {
      if (i == 0) {
        faulty_index = FaultyIndex::AMOUNT;
      }
      else if (i == 1) {
        faulty_index = FaultyIndex::OLD_X;
      }
      else if (i == 2) {
        faulty_index = FaultyIndex::OLD_Y;
      }
      else if (i == 3) {
        faulty_index = FaultyIndex::NEW_X;
      }
      else if (i == 4) {
        faulty_index = FaultyIndex::NEW_Y;
      }
      return false;
    }
    if (std::stoi(command.getParameters()[i]) < 0) {
      if (i == 0) {
        faulty_index = FaultyIndex::AMOUNT;
      }
      else if (i == 1) {
        faulty_index = FaultyIndex::OLD_X;
      }
      else if (i == 2) {
        faulty_index = FaultyIndex::OLD_Y;
      }
      else if (i == 3) {
        faulty_index = FaultyIndex::NEW_X;
      }
      else if (i == 4) {
        faulty_index = FaultyIndex::NEW_Y;
      }
      return false;
    }
  }
  faulty_index = FaultyIndex::CORRECT;
  return true;
}

bool Command::isFloat(Command command, FaultyIndex& faulty_index) {
  for (std::size_t i = 0; i < command.getParameters().size(); i++) {
    for (std::size_t j = 0; j < command.getParameters()[i].size(); j++) {
      if (command.getParameters()[i][j] == '.' || command.getParameters()[i][j] == ',') {
        switch (i) {
          case 0:
            faulty_index = FaultyIndex::AMOUNT;
            return false;
            break;
          case 1:
            faulty_index = FaultyIndex::OLD_X;
            return false;
            break;
          case 2:
            faulty_index = FaultyIndex::OLD_Y;
            return false;
            break;
          case 3:
            faulty_index = FaultyIndex::NEW_X;
            return false;
            break;
          case 4:
            faulty_index = FaultyIndex::NEW_Y;
            return false;
            break;
        }
      }
    }
  }
  return true;
}

void Command::passCommand(Player* curr_player) {
  curr_player->setPassed(true);
}