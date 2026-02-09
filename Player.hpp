#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <cmath>
#include <iostream>

class Player {
  private:
    char id_;
    int chips_;
    int claimed_fields_;
    bool has_passed_;

  public:
    Player(char id);
    Player(Player& player) = delete;
    ~Player() = default;

    void addClaimedFields(int amount);
    void removeClaimedFields(int amount);
    int getClaimedFields();
    void setId(char id);
    char getId();
    void calculateChips();
    int getChips();
    void addChips(int amount);
    void removeChips(int amount);
    void setPassed(bool passed);
    bool getPassed();
};



#endif