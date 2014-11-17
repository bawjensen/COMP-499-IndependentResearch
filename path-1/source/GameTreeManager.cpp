#include "../headers/Board.h"
#include "../headers/GameTreeManager.h"
#include "../headers/NeuralNet.h"

#include <cmath>

using namespace std;

void GameTreeManager::findChildren(const Board& parent, Board*& children, int& numChildren, bool playerMove) {
    if (playerMove) {
        int numPossibleChildren = 4; // One for each direciton (up, left, down, right)

        children = new Board[numPossibleChildren];

        pair<bool, int> result;

        children[0].initialize(); // Initialize first manually

        int i = 0;
        for (int dir = 0; dir < numPossibleChildren; ++dir) {
            children[i] = parent;
            result = children[i].shift(dir);

            if (result.first) { // Bool in pair represents success of move
                children[++i].initialize(); // Shift index and initialize next
            }
        }

        numChildren = i; // Update the number of children to the number of successful moves
    }

    else {
        int numPossibleChildren = 2 * 16; // One for each possible piece to be added, for each slot in table

        children = new Board[numPossibleChildren];

        numChildren = 0;
        int possiblePieces[2] = { 2, 4 };

        children[0].initialize(); // Initialize first manually

        for (int i = 0; i < parent.getWidth(); ++i) {
            for (int j = 0; j < parent.getWidth(); ++j) {
                for (int k = 0; k < 2; ++k) { // Possible pieces
                    children[numChildren] = parent;

                    if (children[numChildren].addPieceManual(i, j, possiblePieces[k])) {
                        children[++numChildren].initialize(); // Shift index and initialize next
                    }
                }
            }
        }

        // numChildren++;
    }
}

float minimax(const Board& board, const NeuralNet& net, int depth, bool maximizing) {
    if (depth == 0) {
        return net.run(board.flatten());
    }

    float bestVal;
    float tempVal;
    if (maximizing) {
        bestVal = -INFINITY;

        int numChildren;
        Board* children;
        GameTreeManager::findChildren(board, children, numChildren, maximizing);

        for (int i = 0; i < numChildren; ++i) {
            tempVal = minimax(children[i], net, depth - 1, false);
            bestVal = max(bestVal, tempVal);
        }
    }
    else {
        bestVal = INFINITY;

        int numChildren;
        Board* children;
        GameTreeManager::findChildren(board, children, numChildren, maximizing);

        for (int i = 0; i < numChildren; ++i) {
            tempVal = minimax(children[i], net, depth - 1, true);
            bestVal = min(bestVal, tempVal);
        }
    }

    return bestVal;
}

int GameTreeManager::determineBestMove(const Board& board, const NeuralNet& net) {
    float bestVal = minimax(board, net, 2, true);
    return bestVal;
}