#include "../headers/TestingSuite.h"
#include "../headers/GameController.h"

#include <cstdlib>

using namespace std;

void TestingSuite::start() {
    int numRuns = 10;

    GameController gc;

    for (int i = 0; i < numRuns; ++i) {
        // (int numNets, int numGenerations, int numGamesPerNet, int netHiddenLayerSize, char chMode)
        gc.initialize(100, 1000, 10, 16, 't');
        gc.start();
    }

}