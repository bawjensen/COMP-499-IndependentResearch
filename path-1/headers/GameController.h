#pragma once

#include "Board.h"
#include "NeuralNet.h"

class GameController {
private:
    Board board;
    int score;
    int numMoves;

    int numNets;
    int numGenerations;
    int numGamesPerNet;

    bool testingNets;

public:
    static bool debug;

    GameController();

    void setTestingNets(bool testingNets) { this->testingNets = testingNets; };
    void setDebug(bool debug) { this->debug = debug; };
    void setNumNets(int numNets) { this->numNets = numNets; };
    void setNumGenerations(int numGenerations) { this->numGenerations = numGenerations; };
    void setNumGamesPerNet(int numGamesPerNet) { this->numGamesPerNet = numGamesPerNet; };

    bool getTestingNets() { return this->testingNets; };
    bool getDebug() { return this->debug; };

    void reset();
    bool gameEnded();
    void start();

    void testNets();
    void runTraining();

    // void runGame();
    int runGameWithNet(NeuralNet& net);
    
    bool handleCommand(char& input);
    std::pair<bool, int> handleCommand(int& direction);
    // bool movesAvailable();
};