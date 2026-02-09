#include "Game.hpp"

Game::Game(int maximum_rounds, char* config_path) {
  player_a_ = new Player('A');
  player_b_ = new Player('B');
  active_player_ = player_a_;
  map_ = new Map(config_path, player_a_, player_b_);
  phase_ = Phase::PLACEMENT;
  current_round_ = 1;
  max_rounds_ = maximum_rounds;
  calcChips();
}

Game::~Game() {
  delete map_;
  delete player_a_;
  delete player_b_;
}

int Game::getRoundNumber(char* string) {
  return atoi(string);
}

bool Game::isValidRoundNumber(int rounds) {
  return rounds > 0;
}

bool Game::isValidConfig(char* config_path) {
  std::ifstream file(config_path);
  std::string line;
  getline(file, line);
  if (line != "OOP") {
    return false;
  }
  return true;
}

void Game::start() {
  std::cout << "Welcome to OOPtimal Tactics!\nPlaying maximum of " << max_rounds_ << " round(s)!" << std::endl;
  printRound();
  printPhase();
  map_->printMap();
  startingPlayer();
  printPlayer();
}

void Game::execute(Command command) {
  switch (phase_) {
    case Phase::PLACEMENT:
      if (command.getType() == CommandType::INFO || command.getType() == CommandType::MAP) {
        goCommand(command);
        printPlayer();
        return;
      }
      if (command.getType() == CommandType::INVALID || command.getType() == CommandType::WRONG_PARAM) {
        printPlayer();
        return;
      }
      if (!goCommand(command)) {
        printPlayer();
        return;
      }
      if (command.getType() == CommandType::PASS) {
        active_player_->setPassed(true);
        if (player_a_->getPassed() && player_b_->getPassed()) {
          switchPhase();
          printPhase();
          map_->printMap();
          printPlayer();
          player_a_->setPassed(false);
          player_b_->setPassed(false);
          return;
        }
        switchPlayer();
        printPlayer();
        return;
      }
      if (active_player_->getChips() == 0) {
        active_player_->setPassed(true);
        map_->printMap();
        if (player_a_->getPassed() && player_b_->getPassed()) {
          startingPlayer();
          switchPlayer();
          switchPhase();
          printPhase();
          map_->printMap();
          printPlayer();
          player_a_->setPassed(false);
          player_b_->setPassed(false);
          return;
        }
        switchPlayer();
        printPlayer();
        return;
      }
      switchPlayer();
      if (active_player_->getPassed()) {
        switchPlayer();
      }
      map_->printMap();
      printPlayer();
      break;

    case Phase::MOVEMENT:
      if (command.getType() == CommandType::INFO || command.getType() == CommandType::MAP) {
        goCommand(command);
        printPlayer();
        return;
      }
      if (command.getType() == CommandType::INVALID || command.getType() == CommandType::WRONG_PARAM) {
        printPlayer();
        return;
      }
      if (!goCommand(command)) {
        printPlayer();
        return;
      }
      if (checkForFields()) {
        phase_ = Phase::END;
        map_->printMap();
        printPhase();
        current_round_ = max_rounds_ + 1;
        isRunning();
        break;
      }
      if (command.getType() == CommandType::PASS) {
        active_player_->setPassed(true);
        if (player_a_->getPassed() && player_b_->getPassed()) {
          switchPhase();
          if (phase_ == Phase::END) {
            printPhase();
            current_round_ = max_rounds_ + 1;
            isRunning();
            break;
          }
          current_round_++;
          calcChips();
          printRound();
          printPhase();
          map_->printMap();
          startingPlayer();
          printPlayer();
          player_a_->setPassed(false);
          player_b_->setPassed(false);
          return;
        }
        switchPlayer();
        printPlayer();
        return;
      }
      switchPlayer();
      if (active_player_->getPassed()) {
        switchPlayer();
      }
      map_->printMap();
      printPlayer();
      break;

    case Phase::END:;
  }
}

bool Game::checkForFields() {
  if (player_a_->getClaimedFields() <= 0) {
    return true;
  }
  if (player_b_->getClaimedFields() <= 0) {
    return true;
  }
  else {
    return false;
  }
}

bool Game::goCommand(Command command) {
  switch (command.getType()) {
    case CommandType::INFO:
      command.printInfo(active_player_);
      return true;
    case CommandType::MAP:
      map_->setOutputActive();
      return true;
    case CommandType::PLACE:
      if (!command.checkPlace(command, map_, active_player_, phase_)) {
        return false;
      }
      command.placeCommand(command, map_, active_player_);
      return true;
    case CommandType::MOVE:
      if (!command.checkMove(command, map_, active_player_, phase_)) {
        return false;
      }
      command.moveCommand(command, map_, active_player_);
      return true;
    case CommandType::PASS:
      command.passCommand(active_player_);
      return true;
    case CommandType::QUIT:
      return true;
    case CommandType::INVALID:
      return false;
    case CommandType::WRONG_PARAM:
      return false;
  }
  return false;
}

void Game::calcChips() {
  player_a_->calculateChips();
  player_b_->calculateChips();
}

void Game::switchPhase() {
  if (current_round_ == max_rounds_ && phase_ == Phase::MOVEMENT) {
    phase_ = Phase::END;
  }
  else if (phase_ == Phase::PLACEMENT) {
    phase_ = Phase::MOVEMENT;
  }
  else {
    phase_ = Phase::PLACEMENT;
  }
}

bool Game::isRunning() {
  return current_round_ <= max_rounds_;
}

void Game::printRound() {
  std::cout << "\n------------------\nRound " << current_round_ << "/" << max_rounds_
            << " starts!\n------------------\n"
            << std::endl;
}

void Game::printPlayer() {
  if (phase_ == Phase::PLACEMENT) {
    std::cout << "Player " << active_player_->getId() << ", you have " << active_player_->getChips()
              << " chip(s) left, where and how do you want to place your chips?" << std::endl;
  }
  else {
    std::cout << "Player " << active_player_->getId() << ", what do you want to do?" << std::endl;
  }
}

void Game::printPhase() {
  std::cout << "------------------" << std::endl;
  switch (phase_) {
    case Phase::PLACEMENT:
      std::cout << "Placement Phase" << std::endl;
      std::cout << "------------------" << std::endl;
      break;
    case Phase::MOVEMENT:
      std::cout << "Movement Phase" << std::endl;
      std::cout << "------------------" << std::endl;
      break;
    case Phase::END:
      std::cout << "GAME END!\n" << std::endl;
      printWinner();
      break;
  }
}

void Game::switchPlayer() {
  if (active_player_ == player_a_) {
    active_player_ = player_b_;
  }
  else {
    active_player_ = player_a_;
  }
}

void Game::startingPlayer() {
  if (current_round_ % 2) {
    active_player_ = player_a_;
  }
  else {
    active_player_ = player_b_;
  }
}

void Game::printWinner() {
  std::cout << "Player A claimed " << player_a_->getClaimedFields() << " field(s)!" << std::endl;
  std::cout << "Player B claimed " << player_b_->getClaimedFields() << " field(s)!" << std::endl;
  Player* winning_player = nullptr;
  bool is_draw = false;
  if (player_a_->getClaimedFields() < player_b_->getClaimedFields()) {
    winning_player = player_b_;
  }
  else if (player_a_->getClaimedFields() > player_b_->getClaimedFields()) {
    winning_player = player_a_;
  }
  else {
    is_draw = true;
  }
  if (!is_draw) {
    std::cout << "\nCongratulations, Player " << winning_player->getId() << "! You won!" << std::endl;
  }
}