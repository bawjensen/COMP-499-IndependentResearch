#include "../headers/GameController.h"

#include <iostream>
#include <chrono>

using namespace std;

GameController::GameController() {
    // this->score = 0;
    this->reset();
}

void GameController::reset() {
    this->score = 0;
    this->numMoves = 0;
}

bool GameController::movesAvailable() {
    return this->board.slotsAvailable() || this->board.matchesPossible();
}

bool GameController::gameEnded() {
    return !this->movesAvailable();
}

void GameController::start() {
    long start = chrono::system_clock::now().time_since_epoch().count();
    srand(start);

    int numRuns = 1;
    int numMoves = 0;

    NeuralNet* nets = new NeuralNet[numRuns];

    for (int i = 0; i < numRuns; ++i) {
        this->reset();
        this->board.reset();
        // this->runGame();
        this->runGameWithNet(nets[i]);
        numMoves += this->numMoves;
    }

    long end = chrono::system_clock::now().time_since_epoch().count();

    float numSec = (end - start) / (float)1000000;
    cout << numMoves << " moves in " << numSec << " sec (" << (int)(numMoves / numSec) << " moves per second)" << endl;
}

void GameController::runGame() {
    for (int i = 0; i < this->numStartingTiles; i++)
        this->board.addRandomTile();

    int input = -1;

    while ( !this->gameEnded() ) {
        if (this->handleCommand(input)) {
            ++this->numMoves;
            this->board.addRandomTile();
        }

        input = rand() % 4;
    }
}

void GameController::runGameWithNet(NeuralNet net) {
    for (int i = 0; i < this->numStartingTiles; i++)
        this->board.addRandomTile();

    int input = -1;
    float netOutput;
    bool success;

    while ( !this->gameEnded() ) {
        success = this->handleCommand(input);

        if (success) {
            ++this->numMoves;
            this->board.addRandomTile();
        }

        if (success) {
            netOutput = net.run(this->board.flatten());
            input = ((int)(netOutput * 4)) % 4;
            cout << "Net returned " << netOutput << endl;
            cout << "Input was " << input << endl;
            cout << "Board was:" << endl << this->board << endl;
        }
        else { // If board can't make a correct move, make a random one for it
            input = rand() % 4;
        }
    }
}

bool GameController::handleCommand(int direction) {
    bool success = false;

    if (direction != -1) {
        pair<bool, int> result = this->board.shift(direction);
        this->score += result.second;
        success = result.first;
    }

    return success;
}