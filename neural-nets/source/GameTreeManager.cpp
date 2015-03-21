#include "../headers/Board.h"
#include "../headers/GameTreeManager.h"
#include "../headers/GameController.h"
#include "../headers/NeuralNet.h"

#include <cmath>
#include <sstream>

using namespace std;

int GameTreeManager::ply = 0;

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

float minimax_alphabeta(const Board& board, const NeuralNet& net, int depth, float alpha, float beta, bool state) {
    // State = 0:
    //     Minimizing random's damage (random addition)
    //     Determining beta using minimum of all children
    //     Uses alpha from parent as ceiling value, computer won't allow more gain
    //     ^ ??? (assumes that the computer will try to thwart player)

    // State = 1:
    //     Maximizing player's gain (directional move)
    //     Determining alpha using maximum of all children
    //     Uses beta from parent as floor value, player won't settle for less

    float bufferVal;

    if ( (depth == 0) || (!board.movesAvailable()) ) {
        bufferVal = evaluateBoard(net, board);
    }
    else {
        int numChildren;
        Board* children = (state == 0) ? new Board[32] : new Board[4];
        GameTreeManager::findChildren(board, children, numChildren, state);

        float tempVal;

        for (int i = 0; i < numChildren; ++i) {
            tempVal = minimax_alphabeta(children[i], net, depth - 1, alpha, beta, !state);

            if (state == 0) beta = min(beta, tempVal);
            else            alpha = max(alpha, tempVal);

            if (beta <= alpha) {
                break;
            }
        }

        bufferVal = (state == 0) ? beta : alpha;

        delete[] children;
    }

    return bufferVal;
}

int GameTreeManager::determineBestMove(const Board& board, const NeuralNet& net) {
    int bestOptionIndex = -1,
        bestOptionDir;

    // Manually perform first layer of minimax_alphabeta, in order to know which direction to go
    float tempVal,
        alpha = -INFINITY,
        beta = INFINITY;

    int numChildren;
    Board* children = new Board[4];
    GameTreeManager::findChildren(board, children, numChildren, true);

    for (int i = 0; i < numChildren; ++i) {
        tempVal = minimax_alphabeta(children[i], net, GameTreeManager::ply - 1, alpha, beta, false);

        if (tempVal > alpha) {
            alpha = tempVal;
            bestOptionIndex = i;
        }
    }

    bestOptionDir = children[bestOptionIndex].getLastMove();

    delete[] children;

    return bestOptionDir;
}