#include "../headers/GameController.h"

#include <iostream>
#include <chrono>

using namespace std;

GameController::GameController() {
    score = 0;
}

void GameController::reset() {
    this->score = 0;
}

bool GameController::movesAvailable() {
    return this->board.slotsAvailable() || this->board.matchesPossible();
}

bool GameController::gameEnded() {
    return !this->movesAvailable();
}

void GameController::start() {
    srand(chrono::system_clock::now().time_since_epoch().count());

    for (int i = 0; i < 10000; ++i) {
        this->reset();
        this->board.reset();
        this->runGame();
        cout << "Got score: " << this->score  << "!" << endl;
    }
}

void GameController::runGame() {
    char inputs[4] = { 'w', 'a', 's', 'd' };

    for (int i = 0; i < this->numStartingTiles; i++)
        this->board.addRandomTile();

    char input = '\0';

    while ( !(input == 'q' || this->gameEnded()) ) {
        if (this->handleCommand(input))
            this->board.addRandomTile();

        // cout << "Current board (" << this->score << "):" << endl;
        // cout << endl << this->board << endl;
        // cout << "Command: ";
        // cin >> input;
        input = inputs[rand() % 4];
    }

    // if (this->gameEnded()) {
    //     // cout << "Game over! Score: " << this->score << endl;
    //     cout << "Final board: " << endl;
    //     cout << endl << this->board << endl;
    // }
}

bool GameController::handleCommand(char input) {
    int direction = InputHandler::convert(input);
    bool success = false;

    if (direction != -1) {
        pair<bool, int> result = this->board.shift(direction);
        this->score += result.second;
        success = result.first;
    }

    return success;
}