#include "../headers/Board.h"
#include "../headers/InputHandler.h"

class GameController {
private:
    Board board;
    InputHandler iHandler;

public:
    GameController();

    void handleInput();
    bool gameEnded();
    void start();
    void runGame();
    void handleCommand(char input);
    void shiftBoard(InputHandler::Direction dir);
};