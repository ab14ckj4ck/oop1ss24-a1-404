#include "Player.hpp"

Player::Player(char id) : id_(id), chips_(0), claimed_fields_(0), has_passed_(false) {}

void Player::addClaimedFields(int amount) {
  claimed_fields_ += amount;
}

void Player::removeClaimedFields(int amount) {
  claimed_fields_ -= amount;
}

int Player::getClaimedFields() {
  return claimed_fields_;
}

void Player::setId(char id) {
  id_ = id;
}

char Player::getId() {
  return id_;
}

void Player::calculateChips() {
  double new_chips = claimed_fields_ / 3.0;
  chips_ += static_cast<int>(std::ceil(new_chips));
}

int Player::getChips() {
  return chips_;
}

void Player::addChips(int amount) {
  chips_ += amount;
}

void Player::removeChips(int amount) {
  chips_ -= amount;
}

void Player::setPassed(bool passed) {
  has_passed_ = passed;
}

bool Player::getPassed() {
  return has_passed_;
}