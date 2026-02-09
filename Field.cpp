#include "Field.hpp"

Field::Field(Player* player, int chips, bool is_water) : player_(player), chips_(chips), is_water_(is_water) {}

void Field::printField() {
  std::cout << "|";
  printFieldPlayer();
  printFieldWater();
  printFieldChips();
}

void Field::setPlayer(Player* player) {
  player_ = player;
}

Player* Field::getPlayer() {
  return player_;
}

bool Field::isWater() {
  return is_water_;
}

void Field::setChips(int amount) {
  chips_ = amount;
}

int Field::getChips() {
  return chips_;
}

void Field::addChips(int amount) {
  chips_ += amount;
}

void Field::printFieldPlayer() {
  if (player_ == nullptr) {
    std::cout << " ";
  }
  else {
    std::cout << player_->getId();
  }
}

void Field::printFieldChips() {
  if (player_ == nullptr) {
    std::cout << " ";
  }
  else {
    std::cout << chips_;
  }
}

void Field::printFieldWater() {
  if (is_water_) {
    std::cout << "~";
  }
  else {
    std::cout << " ";
  }
}