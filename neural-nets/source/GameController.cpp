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

void GameController::runTesting(string filename, int ply, int runs) {
    GameTreeManager::ply = ply;
    srand(chrono::system_clock::now().time_since_epoch().count());

    NeuralNet testNet;
    ifstream netFile(filename);
    if (!netFile.is_open()) throw runtime_error("Issue opening net file for deserialization and testing");
    testNet.deserialize(netFile);


    const int numRuns = runs;

    // int scores[numRuns];
    int totalScore = 0,
        tempScore,
        highest = 0;
    ofstream logFile("testing.log");

    for (int i = 0; i < numRuns; ++i) {
        tempScore = this->runGameWithNet(testNet);

        logFile << tempScore << ',';

        totalScore += tempScore;

        highest = highest < tempScore ? tempScore : highest;

        // scores[i] = tempScore;
    }

    cout << "Average: " << (float)totalScore / numRuns << endl;
    cout << "Highest: " << highest << endl;
}

NeuralNet GameController::runTraining(int numGenerations, int numNets, int numGamesPerNet, int netHiddenLayerSize, char chMode, int treeDepth) {
    cout << "numGenerations,"     << "numNets,"     << "numGamesPerNet,"     << "netHiddenLayerSize,"     << "chMode,"     << "treeDepth,"     << "randomMean,"               << "randomStdDev"         << endl
         << numGenerations << "," << numNets << "," << numGamesPerNet << "," << netHiddenLayerSize << "," << chMode << "," << treeDepth << "," << RandomGen::getMean() << "," << RandomGen::getStdDev() << endl
         << endl;

    NetManager mgr(numNets, netHiddenLayerSize);
    GameTreeManager::ply = treeDepth;
    this->setEvaluationMode(chMode);

    srand(chrono::system_clock::now().time_since_epoch().count());

    int tempScore, // Temp storage of score for the net
        totalScore, // Total score of the generation
        netTotalScore, // Total score of the net in the generation
        netHighest, // Top score of the net in the generation
        genHighest, // Top score of the generation
        overallHighest = 0, // Top score of the entire thing
        bestInGenIndex; // Index of best net in the generation

    NeuralNet bestNet(16, netHiddenLayerSize);
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

            // Get the new best in gen
            if (netHighest > genHighest) {
                genHighest = netHighest;
                bestInGenIndex = j;
            }

            totalScore += netTotalScore;
        }

        cout << i << ","
            << (float)totalScore / (numNets * numGamesPerNet) << ","
            << genHighest << endl;
        logFile << "Generation " << i << " finished." << endl;

        // Update overall top score and store the best net
        if (genHighest > overallHighest) {
            overallHighest = genHighest;
            bestNet = mgr[bestInGenIndex];
        }

        mgr.selectAndMutateSurvivors();
    }

    cout << "Best score overall," << overallHighest << endl;

    return bestNet;
}

int GameController::runGameWithNet(NeuralNet& net) {
    this->board.reset();

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

void GameController::saveNets(NetManager mgr, int numNets) {
    // Serialize and save nets
    for (int i = 0; i < numNets; ++i) {
        ofstream outFile(str(i) + ".net");
        if (!outFile.is_open())
            throw new runtime_error("File to serialize net into didn't open");
        outFile << mgr[i].serialize();
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
