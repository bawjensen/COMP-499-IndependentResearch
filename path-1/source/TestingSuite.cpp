#include "../headers/TestingSuite.h"
#include "../headers/GameController.h"

#include <cstdlib>

using namespace std;

void TestingSuite::start() {
    int numTests = 1;

    GameController gc;

    for (int i = 0; i < numTests; ++i) {
        // (int numGenerations, int numNets, int numGamesPerNet, int netHiddenLayerSize, char chMode)
        gc.initialize(100, 10, 10, 16, 't');
        gc.start();
    }
}