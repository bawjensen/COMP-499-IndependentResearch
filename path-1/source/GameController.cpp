#include "../headers/GameController.h"
#include "../headers/NetManager.h"

#include <iostream>
#include <chrono>

using namespace std;

GameController::GameController() {
    // this->score = 0;
    this->reset();
}

void GameController::reset() {
    this->numMoves = 0;
}

bool GameController::movesAvailable() {
    return this->board.slotsAvailable() || this->board.matchesPossible();
}

bool GameController::gameEnded() {
    return !this->movesAvailable();
}

void GameController::start() {
    int numGenerations = 10;
    int numNets = 4;
    int score;

    // NeuralNet* nets = new NeuralNet[numNets];
    NetManager mgr(numNets);

    mgr.mutateWinners();

    long start = chrono::system_clock::now().time_since_epoch().count();
    srand(start);

    for (int i = 0; i < numGenerations; ++i) {
        for (int j = 0; j < numNets; ++j) {
            this->board.reset();
            score = this->runGameWithNet(mgr[j]);
            mgr.keepScore(score, j);
        }
    }

    long end = chrono::system_clock::now().time_since_epoch().count();

    float numSec = (end - start) / (float)1000000;
    cout << this->numMoves << " moves in " << numSec << " sec (" << (int)(this->numMoves / numSec) << " moves per second)" << endl;
    // delete[] nets;
}

// void GameController::runGame() {
//     for (int i = 0; i < this->numStartingTiles; i++)
//         this->board.addRandomTile();

//     int input = -1;

//     while ( !this->gameEnded() ) {
//         if (this->handleCommand(input)) {
//             ++this->numMoves;
//             this->board.addRandomTile();
//         }

//         input = rand() % 4;
//     }
// }

int GameController::runGameWithNet(NeuralNet& net) {
    for (int i = 0; i < this->numStartingTiles; i++)
        this->board.addRandomTile();

    int input = -1;
    int score = 0;
    float netOutput;
    bool success;
    pair<bool, int> result;

    while ( !this->gameEnded() ) {
        result = this->handleCommand(input);
        success = result.first;
        score += result.second;

        if (success) {
            ++this->numMoves;
            this->board.addRandomTile();
        }

        if (success) {
            netOutput = net.run(this->board.flatten());
            input = (((int)netOutput) * 4) % 4;
            // cout << "Net returned " << netOutput << endl;
            // cout << "Input was " << input << endl;
            // cout << "Board was:" << endl << this->board << endl;
        }
        else { // If board can't make an effective move, make a random one for it
            input = rand() % 4;
        }
    }

    return score;
}

pair<bool, int> GameController::handleCommand(int& direction) {
    pair<bool, int> result;

    if (direction < 4 && direction >= 0) {
        result = this->board.shift(direction);
    }

    return result;
}