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

bool GameController::debug = true;

string str(int i) {
    ostringstream convert;
    convert << i;
    return convert.str();
}

// =============================================================================

GameController::GameController() {
    this->redirectingOutput = false;
    this->reset();
    this->initialize(0, 0, 0, 0, 't', 2);
}

void GameController::reset() {
    this->board.reset();
    this->score = 0;
    this->numMoves = 0;
}

void GameController::initialize(int numGenerations, int numNets, int numGamesPerNet, int netHiddenLayerSize, char chMode, int treeDepth) {
    this->numGenerations = numGenerations;
    this->numNets = numNets;
    this->numGamesPerNet = numGamesPerNet;
    this->netHiddenLayerSize = netHiddenLayerSize;
    this->setEvaluationMode(chMode);
    this->treeDepth = treeDepth;
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

void GameController::start(int numGenerations, int numNets, int numGamesPerNet, int netHiddenLayerSize, char chMode, int treeDepth) {
    this->initialize(numGenerations, numNets, numGamesPerNet, netHiddenLayerSize, chMode, treeDepth);

    cout << "Initialized with: " << endl
        << "numGenerations: " << numGenerations << endl
        << "numNets: " << numNets << endl
        << "numGamesPerNet: " << numGamesPerNet << endl
        << "netHiddenLayerSize: " << netHiddenLayerSize << endl
        << "chMode: " << chMode << endl
        << "treeDepth: " << treeDepth << endl;

    this->start();
}

void GameController::runTraining() {
    this->mgr.initialize(this->numNets, this->netHiddenLayerSize);

    srand(chrono::system_clock::now().time_since_epoch().count());

    int overallTopScore = 0; // Top score of every net in every generation of this setup

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

            mgr.keepScore(tempNetScore, j);

            genHighest = netHighest > genHighest ? netHighest : genHighest;

            totalScore += netTotalScore;
        }
        cout << "Nets of generation " << i << " averaged: "
            << (float)totalScore / (this->numNets * this->numGamesPerNet)
            << " (top score: " << genHighest << ")" << endl;

        // Update overall top score
        overallTopScore = genHighest > overallTopScore ? genHighest : overallTopScore;

        mgr.selectAndMutateSurvivors();
    }

    cout << "Best score overall: " << overallTopScore << endl;
}

int GameController::runGameWithNet(NeuralNet& net) {
    int direction;
    int score = 0;
    float netOutput;
    bool success;
    pair<bool, int> result;

    while ( !this->gameEnded() ) {
        direction = GameTreeManager::determineBestMove(board, net, this->treeDepth);

        result = this->handleCommand(direction);

        if (GameController::debug) cout << "After move: " << endl << board << endl;
        success = result.first;

        if (!success) {
            if (GameController::debug) cout << "Direction: " << direction << " - Board:" << endl << this->board << endl;
            throw runtime_error("Board tried a bad direction");
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
    }
}

void GameController::restoreOutput() {
    if (this->redirectingOutput) {
        this->logFile.close();

        cout.rdbuf(this->coutbuf); // restore old cout to cout

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

pair<bool, int> GameController::handleCommand(const int& direction) {
    pair<bool, int> result;

    if (direction < 4 && direction >= 0)
        result = this->board.shift(direction);
    else {
        cout << "Bad move was: " << direction << endl;
        throw runtime_error("Tried to move in an invalid direction (outside of range 0 - 3)");
    }

    return result;
}
