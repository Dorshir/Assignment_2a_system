//
// Created by HOME on 19/03/2023.
//

#ifndef CARDWAR_A_MAIN_GAME_HPP
#define CARDWAR_A_MAIN_GAME_HPP
class Game{

    Player winner{};

public:
    Game(Player player1, Player player2){}
    ~Game()= default;
    void playTurn();
    void printLastTurn();
    void playAll();
    void printWiner();
    void printLog();
    void printStats();
    static bool player1Won();
    static bool player2Won();
    Player getWinner() {
        return this->winner;
    }
};

void Game::printLastTurn() {

}

void Game::printLog() {}
void Game::playTurn() {}
void Game::printStats() {}
void Game::playAll() {}
bool Game::player1Won() {return false;}
bool Game::player2Won() {return false;}
void Game::printWiner() {}

#endif //CARDWAR_A_MAIN_GAME_HPP
