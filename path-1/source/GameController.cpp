#include "../headers/GameController.h"
#include "../headers/GameTreeManager.h"
#include "../headers/NetManager.h"

#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>

using namespace std;

bool GameController::debug = false;

string str(int i) {
    ostringstream convert;
    convert << i;
    return convert.str();
}

// =============================================================================

GameController::GameController() {
    this->testingNets = false;

    numNets = 0;
    numGenerations = 0;
    numGamesPerNet = 0;

    this->reset();
}

void GameController::reset() {
    this->numMoves = 0;
}

bool GameController::gameEnded() {
    return !this->board.movesAvailable();
}

void GameController::start() {
    if (GameController::testingNets) {
        this->testNets();
    }
    else {
        this->runTraining();
    }
}

void GameController::testNets() {
    ifstream inFile("nets/0.net");
    NeuralNet net;

    if (!inFile.is_open()) {
        cout << "Problem opening serialized net" << endl;
        exit(1);
    }

    net.deserialize(inFile);

    int numRuns = 10;

    long start = chrono::system_clock::now().time_since_epoch().count();
    srand(start);

    this->board.initialize();

    int score;
    for (int i = 0; i < numRuns; ++i) {
        this->board.reset();
        this->board.manualSet(0, 0, 2, 0, 1, 2);
        score = this->runGameWithNet(net);
        cout << "Run " << i << " got score: " << score << endl;
    }

    inFile.close();
}

void GameController::runTraining() {
    NetManager mgr(this->numNets);

    long start = chrono::system_clock::now().time_since_epoch().count();
    srand(start);

    this->board.initialize();
    // this->board.seed();

    int score;
    int totalScore;
    int netTotalScore;
    // Run generations and mutations training
    for (int i = 0; i < this->numGenerations; ++i) {
        totalScore = 0;
        for (int j = 0; j < this->numNets; ++j) {
            netTotalScore = 0;
            for (int k = 0; k < this->numGamesPerNet; ++k) {
                this->board.reset();
                netTotalScore += this->runGameWithNet(mgr[j]);
            }
            mgr.keepScore((float)netTotalScore / this->numGamesPerNet, j);
            totalScore += netTotalScore;
        }
        cout << "Nets of generation " << i << " averaged: " << (float)totalScore / (this->numNets * this->numGamesPerNet) << endl;
        mgr.selectAndMutateSurvivors();
    }

    // Serialize and save nets
    for (int i = 0; i < this->numNets; ++i) {
        ofstream outFile("nets/" + str(i) + ".net");
        outFile << mgr[i].serialize();
        outFile.close();
    }

    long end = chrono::system_clock::now().time_since_epoch().count();

    float numSec = (end - start) / (float)1000000;
    cout << this->numMoves << " moves in " << numSec << " sec (" << (int)(this->numMoves / numSec) << " moves per second)" << endl;
}

// void GameController::runGame() {
//     int direction = -1;

//     while ( !this->gameEnded() ) {
//         if (this->handleCommand(direction)) {
//             ++this->numMoves;
//             this->board.addRandomTile();
//         }

//         direction = rand() % 4;
//     }
// }

int GameController::runGameWithNet(NeuralNet& net) {
    int direction;
    int score = 0;
    float netOutput;
    bool success;
    pair<bool, int> result;

    while ( !this->gameEnded() ) {
        direction = GameTreeManager::determineBestMove(board, net);

        result = this->handleCommand(direction);

        if (GameController::debug) cout << "Move: " << endl << board << endl;
        success = result.first;

        if (!success) {
            cout << "Board tried a bad direction: " << direction << endl;
            cout << this->board << endl;
            exit(1);
        }

        score += result.second;

        ++this->numMoves;
        this->board.addRandomTile();
        if (GameController::debug) cout << "Added: " << endl << board << endl;
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
