#ifndef GAME_HPP
#define GAME_HPP

#include "Map.hpp"
#include "Player.hpp"
#include "Command.hpp"
#include "Phase.hpp"
#include <fstream>
#include <string>


class Game {
  private:
    Map* map_;
    Player* player_a_;
    Player* player_b_;
    Player* active_player_;
    Phase phase_;
    int current_round_;
    int max_rounds_;

  public:
    Game(int maximum_rounds, char* config_path);
    Game(Game& game) = delete;
    ~Game();

    static int getRoundNumber(char* string);
    static bool isValidRoundNumber(int rounds);
    static bool isValidConfig(char* config_path);
    void start();
    void execute(Command command);
    bool isRunning();
    void printRound();
    void printPhase();
    void printPlayer();
    void switchPlayer();
    void startingPlayer();
    void switchPhase();
    void calcChips();
    bool goCommand(Command command);
    void printWinner();
    bool checkForFields();
    
};


#endif