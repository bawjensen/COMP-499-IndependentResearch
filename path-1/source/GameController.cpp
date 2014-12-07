#include "../headers/GameController.h"
#include "../headers/GameTreeManager.h"
#include "../headers/NetManager.h"

#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>

using namespace std;

bool GameController::debug = false;

string str(int i) {
    ostringstream convert;
    convert << i;
    return convert.str();
}

// =============================================================================

GameController::GameController() {
    this->redirectingOutput = false;
    this->reset();
    this->initialize(0, 0, 0, 0, 't');
}

void GameController::reset() {
    this->board.reset();
    this->score = 0;
    this->numMoves = 0;
}

void GameController::initialize(int numGenerations, int numNets, int numGamesPerNet, int netHiddenLayerSize, char chMode) {
    this->numGenerations = numGenerations;
    this->numNets = numNets;
    this->numGamesPerNet = numGamesPerNet;
    this->netHiddenLayerSize = netHiddenLayerSize;
    this->setEvaluationMode(chMode);
}

void GameController::setEvaluationMode(char chMode) {
    switch (chMode) {
        case 'h':
            this->mode = HIGHEST;
            break;
        case 't':
            this->mode = TOTAL;
            break;
        case 'a':
            this->mode = AVERAGE;
            break;
        default:
            string strMode(&chMode, 1);
            throw runtime_error("Impossible mode for evaluation:" + strMode);
    }
}

bool GameController::gameEnded() {
    return !this->board.movesAvailable();
}

void GameController::start() {
    this->runTraining();
}

void GameController::start(int numGenerations, int numNets, int numGamesPerNet, int netHiddenLayerSize, char chMode) {
    this->initialize(numGenerations, numNets, numGamesPerNet, netHiddenLayerSize, chMode);

    cout << "Initialized with: " << endl
        << "numGenerations: " << numGenerations << endl
        << "numNets: " << numNets << endl
        << "numGamesPerNet: " << numGamesPerNet << endl
        << "netHiddenLayerSize: " << netHiddenLayerSize << endl
        << "chMode: " << chMode << endl;

    this->start();
}

// void GameController::testNets() {
//     ifstream inFile("nets/11-24-14_32/0.net");
//     NeuralNet net;

//     if (!inFile.is_open()) {
//         throw runtime_error("Problem opening serialized net");
//     }

//     net.deserialize(inFile);
//     inFile.close();

//     int numRuns = 1000;

//     int highest = -INFINITY;
//     int lowest = INFINITY;
//     int total = 0;

//     auto start = chrono::steady_clock::now();
//     srand(start.time_since_epoch().count());

//        this->board.initialize();

//     int score;
//     for (int i = 0; i < numRuns; ++i) {
//         this->board.reset();
//         this->board.manualSet(0, 0, 2, 0, 1, 2);
//         score = this->runGameWithNet(net);
//         total += score;
//         highest = score > highest ? score : highest;
//         lowest = score < lowest ? score : lowest;
//         // cout << "Run " << i << " got score: " << score << endl;
//     }

//     auto end = chrono::steady_clock::now();

//     cout << "Net stats:" << endl;
//     cout << "highest: " << highest << endl;
//     cout << "lowest:  " << lowest << endl;
//     cout << "average: " << total / (float)numRuns << endl;

//     double numSec = chrono::duration<double>(end - start).count();
//     cout << numRuns << " games in " << numSec << " sec (" << (int)(numRuns / numSec) << " games per second)" << endl;

// }

void GameController::runTraining() {
    this->mgr.initialize(this->numNets, this->netHiddenLayerSize);

    srand(chrono::system_clock::now().time_since_epoch().count());

    // this->board.initialize();
    // this->board.seed();

    int score, totalScore, netTotalScore, netHighest, tempScore, genHighest;
    float tempNetScore;
    // Run generations and mutations training
    for (int i = 0; i < this->numGenerations; ++i) {
        totalScore = 0;
        genHighest = 0;

        for (int j = 0; j < this->numNets; ++j) {
            netTotalScore = 0;
            netHighest = 0;

            for (int k = 0; k < this->numGamesPerNet; ++k) {
                this->board.reset();
                tempScore = this->runGameWithNet(mgr[j]);
                netTotalScore += tempScore;

                netHighest = tempScore > netHighest ? tempScore : netHighest;
            }

            switch (this->mode) {
                case HIGHEST:
                    tempNetScore = netHighest;
                    break;
                case AVERAGE:
                    tempNetScore = (float)netTotalScore / this->numGamesPerNet;
                    break;
                case TOTAL:
                    tempNetScore = netTotalScore;
                    break;
            }

            // cout << "tempNetScore: " << tempNetScore << endl;

            mgr.keepScore(tempNetScore, j);

            genHighest = netHighest > genHighest ? netHighest : genHighest;
            
            totalScore += netTotalScore;
        }
        cout << "Nets of generation " << i << " averaged: "
            << (float)totalScore / (this->numNets * this->numGamesPerNet)
            << " (top score: " << genHighest << ")" << endl;
        mgr.selectAndMutateSurvivors();
    }
}

// void GameController::runGame() {
//     int direction = -1;

//     while ( !this->gameEnded() ) {
//         if (this->handleCommand(direction)) {
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
            cout << this->board << endl;
            throw runtime_error("^Board tried a bad direction");
        }

        score += result.second;

        this->board.addRandomTile();
        if (GameController::debug) cout << "Added: " << endl << board << endl;
    }

    return score;
}

void GameController::redirectOutputTo(string logFilePath) {
    if (!this->redirectingOutput) { // If not already redirecting output
        this->logFile.open(logFilePath);

        if (!this->logFile.is_open()) throw runtime_error("Could not open file: " + logFilePath);

        this->coutbuf = cout.rdbuf(); // save old buf
        cout.rdbuf(this->logFile.rdbuf()); // redirect cout to log

        this->redirectingOutput = true;
    }
    else { // If already redirecting output;
        this->logFile.close();
        this->logFile.open(logFilePath);

        if (!this->logFile.is_open()) throw runtime_error("Could not open file: " + logFilePath);

        cout.rdbuf(this->logFile.rdbuf()); // redirect cout to log
        
        this->redirectingOutput = true;
    }
}

void GameController::restoreOutput() {
    if (this->redirectingOutput) {
        cout.rdbuf(this->coutbuf); // restore 
        this->redirectingOutput = false;
    }
    else {
        throw runtime_error("Cannot restore output that hasn't been redirected");
    }
}

void GameController::saveNetsTo(string outputDir) {
    // Serialize and save nets
    for (int i = 0; i < this->numNets; ++i) {
        ofstream outFile(outputDir + "/" + str(i) + ".net");
        outFile << this->mgr[i].serialize();
        outFile.close();
    }
}

pair<bool, int> GameController::handleCommand(int& direction) {
    pair<bool, int> result;

    if (direction < 4 && direction >= 0) {
        result = this->board.shift(direction);
    }

    return result;
}
