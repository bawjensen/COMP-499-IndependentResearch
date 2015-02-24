#pragma once

#include "Board.h"
#include "NeuralNet.h"
#include "NetManager.h"

#include <fstream>

enum EMode {
    HIGHEST,
    TOTAL,
    AVERAGE // No real difference from total
};

class GameController {
private:
    Board board;
    NetManager mgr;
    int score;
    int numMoves;

    bool redirectingOutput;
    std::ofstream logFile;
    std::streambuf *coutbuf; // old buf location

    int numNets;
    int numGenerations;
    int numGamesPerNet;
    int netHiddenLayerSize;
    EMode mode;
    int treeDepth;

public:
    static bool debug;

    GameController();
    GameController(int numGenerations, int numNets, int numGamesPerNet, int netHiddenLayerSize, char chMode, int treeDepth);

    void reset();
    void initialize(int numGenerations, int numNets, int numGamesPerNet, int netHiddenLayerSize, char chMode, int treeDepth);

    void setDebug(bool debug) { this->debug = debug; };
    void setNumNets(int numNets) { this->numNets = numNets; };
    void setNumGenerations(int numGenerations) { this->numGenerations = numGenerations; };
    void setNumGamesPerNet(int numGamesPerNet) { this->numGamesPerNet = numGamesPerNet; };
    void setNetHiddenLayerSize(int netHiddenLayerSize) { this->netHiddenLayerSize = netHiddenLayerSize; };
    void setEvaluationMode(char chMode);

    bool getDebug() { return this->debug; };
    int getNumNets() { return this->numNets; };
    int getNumGenerations() { return this->numGenerations; };
    int getNumGamesPerNet() { return this->numGamesPerNet; };
    int getNetHiddenLayerSize() { return this->netHiddenLayerSize; };

    bool gameEnded();
    void start();
    void start(int numGenerations, int numNets, int numGamesPerNet, int netHiddenLayerSize, char chMode, int treeDepth);

    // void testNets();
    void runTraining();

    // void runGame();
    int runGameWithNet(NeuralNet& net);
    
    void redirectOutputTo(std::string logFilePath);
    void restoreOutput();
    void saveNetsTo(std::string outputDir);
    bool handleCommand(char& input);
    std::pair<bool, int> handleCommand(const int& direction);
    // bool movesAvailable();
};