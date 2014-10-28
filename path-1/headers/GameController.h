#include "Board.h"
#include "InputHandler.h"

class GameController {
private:
    Board board;
    InputHandler iHandler;
    const int numStartingTiles = 2;

public:
    GameController();

    void handleInput();
    bool gameEnded();
    void start();
    void runGame();
    void handleCommand(char input);
    void shiftBoard(InputHandler::Direction dir);
};