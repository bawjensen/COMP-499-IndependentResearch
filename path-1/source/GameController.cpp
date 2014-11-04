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

    int numRuns = 10000;
    int numMoves = 0;

    for (int i = numRuns; i != 0; --i) {
        this->reset();
        this->board.reset();
        this->runGame();
        numMoves += this->numMoves;
        // cout << endl << this->board << endl;
        // cout << "Got score: " << this->score  << "!" << endl;
    }

    long end = chrono::system_clock::now().time_since_epoch().count();

    float numSec = (end - start) / (float)1000000;
    cout << numMoves << " moves in " << numSec << " sec (" << numMoves / numSec << " moves per second)" << endl;
}

void GameController::runGame() {
    // char inputs[4] = { 'w', 'a', 's', 'd' };

    for (int i = 0; i < this->numStartingTiles; i++)
        this->board.addRandomTile();

    int input = -1;

    while ( !this->gameEnded() ) {
        if (this->handleCommand(input)) {
            ++this->numMoves;
            this->board.addRandomTile();
        }

        // this->board.wipeMergedStatus();

        // cout << "Current board (" << this->score << "):" << endl;
        // cout << endl << this->board << endl;
        // cout << "Command: ";
        // cin >> input;
        input = rand() % 4;
    }

    // if (this->gameEnded()) {
    //     // cout << "Game over! Score: " << this->score << endl;
    //     cout << "Final board: " << endl;
    //     cout << endl << this->board << endl;
    // }
}

bool GameController::handleCommand(char input) {
    int direction = InputHandler::convert(input);

    return this->handleCommand(direction);
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