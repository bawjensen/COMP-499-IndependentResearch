#include "Board.h"
#include "NeuralNet.h"

#include <iostream>

class GameTreeManager {
private:
public:
    static int determineBestMove(const Board& board, const NeuralNet& net);
    static void findChildren(const Board& parent, Board*& children, int& numChildren, bool playerMove);
};