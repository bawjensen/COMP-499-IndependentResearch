#include "Board.h"
#include "InputHandler.h"

class GameController {
private:
    Board board;
    const int numStartingTiles = 2;
    int score;
    int numMoves;

public:
    GameController();

    void reset();
    bool gameEnded();
    void start();
    void runGame();
    bool handleCommand(char input);
    bool handleCommand(int direction);
    bool movesAvailable();
};