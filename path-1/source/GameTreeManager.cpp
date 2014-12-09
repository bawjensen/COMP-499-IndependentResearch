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

float minimax(const Board& board, const NeuralNet& net, int depth, bool maximizing) {
    float bestVal;
    if (depth == 0) {
        // bestVal = net.run(board.flatten());
        bestVal = net.run(board.flattenNormalize());
        // cout << "Board evalled to: " << bestVal << endl << printArray(board.flatten(), 16) << endl;
        // cout << "Board evalled to: " << bestVal << endl << printArray(board.flattenNormalize(), 16) << endl;
    }
    else if (maximizing) {
        float tempVal;
        bestVal = -INFINITY;

        int numChildren;
        Board* children = new Board[4];
        GameTreeManager::findChildren(board, children, numChildren, maximizing);

        for (int i = 0; i < numChildren; ++i) {
            tempVal = minimax(children[i], net, depth - 1, false);
            bestVal = max(bestVal, tempVal);
        }

        delete[] children;
    }
    else {
        float tempVal;
        bestVal = INFINITY;

        int numChildren;
        Board* children = new Board[32];
        GameTreeManager::findChildren(board, children, numChildren, maximizing);

        for (int i = 0; i < numChildren; ++i) {
            tempVal = minimax(children[i], net, depth - 1, true);
            bestVal = min(bestVal, tempVal);
        }

        delete[] children;
    }

    return bestVal;
}

int GameTreeManager::determineBestMove(const Board& board, const NeuralNet& net, int treeDepth) {
    int bestOption = -1;

    // Manually perform first layer of minimax, in order to know which direction to go
    float tempVal,
        bestVal = -INFINITY;

    int numChildren;
    Board* children = new Board[4];
    GameTreeManager::findChildren(board, children, numChildren, true);

    if (GameController::debug) cout << "Parent: " << endl << board << endl;
    if (GameController::debug) cout << "numChildren: " << numChildren << endl;

    for (int i = 0; i < numChildren; ++i) {
        if (GameController::debug) cout << "Child " << i << ": " << endl << board << endl;
    }

    for (int i = 0; i < numChildren; ++i) {
        tempVal = minimax(children[i], net, treeDepth - 1, false);

        if (tempVal > bestVal) {
            bestVal = tempVal;
            bestOption = i;
        }
    }

    delete[] children;

    return children[bestOption].getLastMove();
}