#include "../headers/Board.h"

// #include <vector>

using namespace std;

Board::Board() {
    this->setWidth(4);
    this->initialize();
}

// Call default constructor before doing anything else
Board::Board(int width) {
    this->setWidth(width);
    this->initialize();
}

void Board::initialize() {
    this->width = width;

    this->board = new int*[this->width];

    for (int i = 0; i < this->width; i++) {
        this->board[i] = new int[this->width];
        for (int j = 0; j < this->width; j++) {
            this->board[i][j] = 0;
        }
    }
}

vector<int*> Board::getAvailableCells() {
    vector<int*> openCells;

    for (int i = 0; i < this->width; i++) {
        for (int j = 0; j < this->width; j++) {
            if (this->board[i][j] == 0) {
                int xyPair[2] = { i, j };
                openCells.push_back(xyPair);
            }
        }
    }

    return openCells;
}

bool Board::addPiece(int x, int y) {
    srand(time(NULL));

    bool success;
    if (this->board[x][y] != 0) {
        success = false;
    }
    else {
        success = true;
        this->board[x][y] = (rand() % 10) < 9 ? 2 : 4;
    }

    return success;
}

void Board::addRandomTile() {
    srand(time(NULL));

    int randX = rand() % 4;
    int randY = rand() % 4;

    while (this->board[randX][randY] != 0) {
        randX = rand() % 4;
        randY = rand() % 4;
    }

    bool success = this->addPiece(randX, randY);
    if (!success) {
        cout << "Error with adding piece" << endl;
        exit(1);
    }
}

ostream& operator<<(ostream& out, Board& board) {
    for (int i = 0; i < board.width; i++) {
        for (int j = 0; j < board.width; j++) {
            out << board.board[i][j] << " ";
        }
        out << endl;
    }

    return out;
}