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
        for (int j = 0; j < this->board.getWidth(); ++j) {
            
            for (int i = this->board.getWidth()-2; i >= 0; --i) {

            }
        }
    }
}