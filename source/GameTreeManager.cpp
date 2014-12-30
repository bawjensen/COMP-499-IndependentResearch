#include "../headers/Board.h"
#include "../headers/GameTreeManager.h"
#include "../headers/GameController.h"
#include "../headers/NeuralNet.h"

#include <cmath>
#include <sstream>

using namespace std;

string printArray(float* array, int size) {
    ostringstream buffer;
    for (int i = 0; i < size; ++i) {
        buffer << array[i] << " ";
    }

    return buffer.str();
}

// ------------------------------------------------------------------------------------------------

void GameTreeManager::findChildren(const Board& parent, Board* children, int& numChildren, bool playerMove) {
    if (playerMove) {
        pair<bool, int> result;

        int i = 0;
        for (int dir = 0; dir < 4; ++dir) { // 4 directions
            children[i] = parent;
            result = children[i].shift(dir);

            if (result.first) { // Bool in pair represents success of move
                ++i; // Shift index
            }
        }

        numChildren = i; // Update the number of children to the number of successful moves
    }

    else {
        numChildren = 0;
        int possiblePieces[2] = { 2, 4 };

        for (int i = 0; i < parent.getWidth(); ++i) {
            for (int j = 0; j < parent.getWidth(); ++j) {
                for (int k = 0; k < 2; ++k) { // Possible pieces
                    children[numChildren] = parent;

                    if (children[numChildren].addPieceManual(i, j, possiblePieces[k])) {
                        ++numChildren; // Shift index
                    }
                }
            }
        }
    }
}

float evaluateBoard(const NeuralNet& net, const Board& board) {
    return net.run(board.flattenNormalize());
}

float minimax(const Board& board, const NeuralNet& net, int depth, bool state) {
    // State = 0: Maximizing 

    float bestVal;

    if (depth == 0) {
        bestVal = evaluateBoard(net, board);
    }
    else {
        float tempVal;
        bestVal = (state == 1) ? -INFINITY : INFINITY;

        int numChildren;
        Board* children = (state == 1) ? new Board[4] : new Board[32];
        GameTreeManager::findChildren(board, children, numChildren, state);

        if (numChildren == 0) {
            bestVal = evaluateBoard(net, board);
        }
        else {
            for (int i = 0; i < numChildren; ++i) {
                tempVal = minimax(children[i], net, depth - 1, !state);
                bestVal = (state == 1) ? max(bestVal, tempVal) : min(bestVal, tempVal);
            }
        }

        delete[] children;
    }

    return bestVal;
}

int GameTreeManager::determineBestMove(const Board& board, const NeuralNet& net, int depth) {
    int bestOptionIndex = -1,
        bestOptionDir;

    // Manually perform first layer of minimax, in order to know which direction to go
    float tempVal,
        bestVal = -INFINITY;

    int numChildren;
    Board* children = new Board[4];
    GameTreeManager::findChildren(board, children, numChildren, true);

    if (GameController::debug) cout << "Parent: " << endl << board << endl;
    if (GameController::debug) cout << "numChildren: " << numChildren << endl;

    for (int i = 0; i < numChildren; ++i) {
        tempVal = minimax(children[i], net, depth - 1, false);

        if (GameController::debug) cout << "Depth: " << depth << " - score: " << tempVal << endl;

        if (tempVal > bestVal) {
            bestVal = tempVal;
            bestOptionIndex = i;
        }
    }

    bestOptionDir = children[bestOptionIndex].getLastMove();

    delete[] children;

    return bestOptionDir;
}