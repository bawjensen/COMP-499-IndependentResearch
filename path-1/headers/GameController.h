#pragma once

#include "Board.h"
#include "NeuralNet.h"

class GameController {
private:
    Board board;
    const int numStartingTiles = 2;
    int score;
    int numMoves;

public:
    GameController();

    void reset();
    bool gameEnded();
    void start();
    // void runGame();
    int runGameWithNet(NeuralNet& net);
    bool handleCommand(char& input);
    std::pair<bool, int> handleCommand(int& direction);
    bool movesAvailable();
};