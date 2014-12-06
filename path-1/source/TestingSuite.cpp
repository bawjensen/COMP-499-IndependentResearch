#include "../headers/TestingSuite.h"
#include "../headers/GameController.h"

#include <fstream>

using namespace std;

long TestingSuite::numMoves = 0;

void TestingSuite::start(string configFile) {
    int numTests = 1;

    GameController gc;

    auto start = chrono::steady_clock::now();

    ifstream inFile(configFile);

    if (!inFile.is_open()) {
        throw runtime_error("File path invalid: " + configFile);
    }

    for (int i = 0; i < numTests; ++i) {
        // Current (from method declaration):
        // (int numGenerations, int numNets, int numGamesPerNet, int netHiddenLayerSize, char chMode)

        // Needed (from README):
        // + Generations
        // + Nets
        // + Games
        // + Hidden Size
        // + Survivor Choice Method
        // o Mutation Rate (random boolean to determine whether or not to mutate)
        // o Mutation Method (different distributions, different std dev's)
        // o Random insertion of newcomers (yes/no, possibly how many/how often)
        // o Cross-mutation (two parents instead of the current one)
        // o Depth of game tree

        gc.start(1000, 100, 10, 16, 'h');
    }

    auto end = chrono::steady_clock::now();

    double numSec = chrono::duration<double>(end - start).count();
    cout << TestingSuite::numMoves << " moves in " << numSec << " sec (" << (int)(TestingSuite::numMoves / numSec) << " moves per second)" << endl;
}