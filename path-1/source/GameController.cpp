#include "../headers/GameController.h"
#include "../headers/GameTreeManager.h"
#include "../headers/NetManager.h"

#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>

using namespace std;

string str(int i) {
    ostringstream convert;
    convert << i;
    return convert.str();
}

GameController::GameController() {
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

void GameController::testNetByName(string netFileName) {
    ifstream inFile("nets/" + netFileName);
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
        // cout << "Starting board: " << endl << board << endl;
        score = this->runGameWithNet(net);
        cout << "Run " << i << " got score: " << score << endl;
    }

    // this->board.reset();

    // int width = this->board.getWidth();

    // for (int i1 = 0; i1 < width; ++i1) {
    //     for (int j1 = 0; j1 < width; ++j1) {
    //         for (int i2 = 0; i2 < width; ++i2) {
    //             for (int j2 = 0; j2 < width; ++j2) {
    //                 if (i1 == i2 || j1 == j2) continue;
    //                 this->board.manualSet(i1, j1, 2, i2, j2, 2);
    //                 float eval = net.run(this->board.flatten());
    //                 cout << i1 << ", " << j1 << " - " << i2 << ", " << j2 << " evalled to: " << eval << endl;
    //             }
    //         }
    //     }
    // }

    inFile.close();
}

void GameController::start() {
    int numGenerations = 10000;
    int numNets = 100;

    NetManager mgr(numNets);

    long start = chrono::system_clock::now().time_since_epoch().count();
    srand(start);

    this->board.initialize();
    // this->board.seed();

    int score;
    int avgScore;
    // Run generations and mutations training
    for (int i = 0; i < numGenerations; ++i) {
        avgScore = 0;
        for (int j = 0; j < numNets; ++j) {
            this->board.reset();
            score = this->runGameWithNet(mgr[j]);
            mgr.keepScore(score, j);
            // cout << "Net (" << &mgr[j] << ") scored: " << score << endl;
            // cout << "Final board: " << endl << this->board << endl;
            avgScore += score;
        }
        cout << "Nets of generation " << i << " averaged: " << (float)avgScore / numNets << endl;
        mgr.selectAndMutateSurvivors();
    }

    // Serialize and save nets
    for (int i = 0; i < numNets; ++i) {
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

    direction = GameTreeManager::determineBestMove(board, net);

    while ( !this->gameEnded() ) {
        // cout << "Going direction: " << direction << endl;
        result = this->handleCommand(direction);
        success = result.first;

        if (!success) {
            cout << "Board tried a bad direction: " << direction << endl;
            cout << this->board << endl;
            exit(1);
        }

        score += result.second;

        ++this->numMoves;
        this->board.addRandomTile();

        direction = GameTreeManager::determineBestMove(board, net);
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