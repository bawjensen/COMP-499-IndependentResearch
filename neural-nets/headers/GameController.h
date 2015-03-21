#pragma once

#include "Board.h"
#include "NeuralNet.h"
#include "NetManager.h"

#include <fstream>

enum EvalMode {
    HIGHEST,
    TOTAL,
    AVERAGE // No real difference from total
};

class GameController {
private:
    Board board;
    NetManager mgr;
    int score;

    bool redirectingOutput;
    std::ofstream logFile;
    std::streambuf *coutbuf; // old buf location

    EvalMode mode;

public:
    static bool debug;
    static int numMoves;

    GameController();
    GameController(int numGenerations, int numNets, int numGamesPerNet, int netHiddenLayerSize, char chMode, int treeDepth);

    void reset();

    void setDebug(bool debug) { this->debug = debug; };
    void setEvaluationMode(char chMode);

    bool getDebug() { return this->debug; };

    bool gameEnded();

    void runTesting(std::string filename, int ply);
    NeuralNet runTraining(int numGenerations, int numNets, int numGamesPerNet, int netHiddenLayerSize, char chMode, int treeDepth);

    int runGameWithNet(NeuralNet& net);
    
    void redirectOutputTo(std::string logFilePath);
    void restoreOutput();
    void saveNets(int numNets);
    bool handleCommand(char& input);
    std::pair<bool, int> handleCommand(const int& direction);
};