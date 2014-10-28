#include "../headers/GameController.h"

#include <iostream>

using namespace std;

GameController::GameController() {
}

void GameController::handleInput() {

}

bool GameController::gameEnded() {
    return false;
}

void GameController::start() {
    this->runGame();
}

void GameController::runGame() {
    char input;

    for (int i = 0; i < this->numStartingTiles; i++)
        this->board.addRandomTile();
    cout << "Starting board:" << endl;
    cout << endl << this->board << endl;

    cout << "Command: ";
    cin >> input;

    while ( !(input == 'q' || this->gameEnded()) ) {
        this->handleCommand(input);
        this->board.addRandomTile();

        cout << endl << this->board << endl;
        cout << "Command: ";
        cin >> input;
    }
}

void GameController::handleCommand(char input) {
    this->shiftBoard(iHandler.convert(input));
}

void GameController::shiftBoard(InputHandler::Direction dir) {
    if (dir == InputHandler::DOWN) {
        this->board.shiftDown();
    }
}