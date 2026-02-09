#ifndef FIELD_HPP
#define FIELD_HPP

#include "Player.hpp"
#include <iostream>

class Field {
  private:
    Player* player_;
    int chips_;
    bool is_water_;

  public:
    Field(Player* player, int chips, bool is_water);
    Field(Field& field) = delete;
    ~Field() = default;

    void setPlayer(Player* player);
    Player* getPlayer();
    bool isWater();
    void setChips(int amount);
    int getChips();
    void addChips(int amount);
    void printField();
    void printFieldPlayer();
    void printFieldChips();
    void printFieldWater();
};

#endif