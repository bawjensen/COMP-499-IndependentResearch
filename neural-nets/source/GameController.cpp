#include "../headers/GameController.h"
#include "../headers/GameTreeManager.h"
#include "../headers/RandomGen.h"

#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>

using namespace std;

bool GameController::debug = false;
int GameController::numMoves = 0;

string str(int i) {
    ostringstream convert;
    convert << i;
    return convert.str();
}

// =============================================================================

GameController::GameController() {
    this->redirectingOutput = false;
    this->reset();
}

void GameController::reset() {
    this->board.reset();
    this->score = 0;
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

void GameController::runTesting() {
    NeuralNet testNet;
    ifstream netFile("saved-runs/3-21-15_test/1.net");

    testNet.deserialize(netFile);

    srand(chrono::system_clock::now().time_since_epoch().count());

    const int numRuns = 100;

    int scores[numRuns];
    int tempScore;
    ofstream logFile("testing.log");

    for (int i = 0; i < numRuns; ++i) {
        tempScore = this->runGameWithNet(testNet);

        logFile << "Score: " << tempScore << endl;

        scores[i] = tempScore;
    }
}

void GameController::runTraining(int numGenerations, int numNets, int numGamesPerNet, int netHiddenLayerSize, char chMode, int treeDepth) {
    cout << "numGenerations,"     << "numNets,"     << "numGamesPerNet,"     << "netHiddenLayerSize,"     << "chMode,"     << "treeDepth,"     << "randomMean,"               << "randomStdDev"         << endl
         << numGenerations << "," << numNets << "," << numGamesPerNet << "," << netHiddenLayerSize << "," << chMode << "," << treeDepth << "," << RandomGen::getMean() << "," << RandomGen::getStdDev() << endl
         << endl;

    this->mgr.initialize(numNets, netHiddenLayerSize);
    GameTreeManager::ply = treeDepth;
    this->setEvaluationMode(chMode);

    srand(chrono::system_clock::now().time_since_epoch().count());

    int overallTopScore = 0; // Top score of every net in every generation of this setup

    int tempScore, // Temp storage of score for the net
        totalScore, // Total score of the generation
        netTotalScore, // Total score of the net in the generation
        netHighest, // Top score of the net in the generation
        genHighest; // Top score of the generation
    float tempNetScore;

    ofstream logFile("training.log");

    // Run generations and mutations training
    for (int i = 0; i < numGenerations; ++i) {
        totalScore = 0;
        genHighest = 0;

        for (int j = 0; j < numNets; ++j) {
            netTotalScore = 0;
            netHighest = 0;

            for (int k = 0; k < numGamesPerNet; ++k) {
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
                    tempNetScore = (float)netTotalScore / numGamesPerNet;
                    break;
                case TOTAL:
                    tempNetScore = netTotalScore;
                    break;
            }

            mgr.keepScore(tempNetScore, j);

            genHighest = netHighest > genHighest ? netHighest : genHighest;

            totalScore += netTotalScore;
        }

        cout << i << ","
            << (float)totalScore / (numNets * numGamesPerNet) << ","
            << genHighest << endl;
        logFile << "Generation " << i << " finished." << endl;

        // Update overall top score
        overallTopScore = genHighest > overallTopScore ? genHighest : overallTopScore;

        mgr.selectAndMutateSurvivors();
    }

    cout << "Best score overall," << overallTopScore << endl;
}

int GameController::runGameWithNet(NeuralNet& net) {
    int direction;
    int score = 0;
    float netOutput;
    bool success;
    pair<bool, int> result;

    while ( !this->gameEnded() ) {
        direction = GameTreeManager::determineBestMove(board, net);

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

void GameController::saveNets(int numNets) {
    // Serialize and save nets
    for (int i = 0; i < numNets; ++i) {
        ofstream outFile(str(i) + ".net");
        if (!outFile.is_open())
            throw new runtime_error("File to serialize net into didn't open");
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
