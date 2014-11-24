#pragma once

#include "Board.h"
#include "NeuralNet.h"

enum EMode {
    HIGHEST,
    TOTAL,
    AVERAGE // No real difference from total
};

class GameController {
private:
    Board board;
    int score;
    int numMoves;

    int numNets;
    int numGenerations;
    int numGamesPerNet;
    int netHiddenLayerSize;

    EMode mode;

    bool testingNets;

public:
    static bool debug;

    GameController();

    void setTestingNets(bool testingNets) { this->testingNets = testingNets; };
    void setDebug(bool debug) { this->debug = debug; };
    void setNumNets(int numNets) { this->numNets = numNets; };
    void setNumGenerations(int numGenerations) { this->numGenerations = numGenerations; };
    void setNumGamesPerNet(int numGamesPerNet) { this->numGamesPerNet = numGamesPerNet; };
    void setNetHiddenLayerSize(int netHiddenLayerSize) { this->netHiddenLayerSize = netHiddenLayerSize; };
    void setEvaluationMode(char chMode);

    bool getTestingNets() { return this->testingNets; };
    bool getDebug() { return this->debug; };
    int getNumNets() { return this->numNets; };
    int getNumGenerations() { return this->numGenerations; };
    int getNumGamesPerNet() { return this->numGamesPerNet; };
    int getNetHiddenLayerSize() { return this->netHiddenLayerSize; };

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