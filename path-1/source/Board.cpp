#include "../headers/Board.h"

#include <iomanip>

using namespace std;

Board::Board() {
    this->initialized = false;
}

Board::~Board() {
    this->destroy();
}

void Board::initialize() {
    if (this->initialized) {
        cout << "ERROR: Trying to reinitialize" << endl;
        exit(1);
    }

    this->width = 4;
    this->board = new Tile*[this->width];

    for (int i = 0; i < this->width; ++i) {
        this->board[i] = new Tile[this->width];
    }

    this->initialized = true;
}

void Board::wipe() {
    for (int i = 0; i < this->width; ++i) {
        for (int j = 0; j < this->width; ++j) {
            this->board[i][j] = 0;
        }
    }
}

void Board::seed() {
    for (int i = 0; i < this->numStartingTiles; ++i) {
        this->addRandomTile();
    }
}

void Board::destroy() {
    if (this->initialized) {
        for (int i = 0; i < this->width; ++i) {
            delete[] this->board[i];
        }
        delete[] this->board;

        this->initialized = false;
    }
}

void Board::reset() {
    // this->destroy();
    if (!this->initialized) this->initialize();
    this->wipe();
    this->seed();
}

void Board::manualSet(int x1, int y1, int val1, int x2, int y2, int val2) {
    if (!this->initialized) this->initialize();

    this->wipe();

    this->board[x1][y1] = val1;
    this->board[x2][y2] = val2;
}


float* Board::flatten() const {
    float* flattened = new float[this->width * this->width];

    for (int i = 0; i < this->width; ++i) {
        for (int j = 0; j < this->width; ++j) {
            flattened[ (i * this->width) + j ] = this->board[i][j].getValue();
        }
    }

    return flattened;
}

vector<int*> Board::getAvailableCells() {
    vector<int*> openCells;

    for (int i = 0; i < this->width; i++) {
        for (int j = 0; j < this->width; j++) {
            if (this->board[i][j].isEmpty()) {
                int xyPair[2] = { i, j };
                openCells.push_back(xyPair);
            }
        }
    }

    return openCells;
}

void Board::addPiece(int x, int y) {
    this->addPieceManual(x, y, (rand() % 10) < 9 ? 2 : 4);
}

bool Board::addPieceManual(int x, int y, int value) {
    if (this->board[x][y].isEmpty()) {
        this->board[x][y] = value;
        return true;
    }
    else {
        return false;
    }
}

void Board::addRandomTile() {
    int randX = rand() % 4;
    int randY = rand() % 4;

    while (!this->board[randX][randY].isEmpty()) {
        randX = rand() % 4;
        randY = rand() % 4;
    }

    this->addPiece(randX, randY);
}

pair<int, int> Board::getVector(int dir) {
    pair<int, int> vec;

    switch(dir) {
        case 0: // Up
            vec.first  = -1;
            vec.second = 0;
            break;
        case 1: // Left
            vec.first  = 0;
            vec.second = -1;
            break;
        case 2: // Down
            vec.first  = 1;
            vec.second = 0;
            break;
        case 3: // Right
            vec.first  = 0;
            vec.second = 1;
            break;
    }

    return vec;
}

void Board::getXTraversals(pair<int, int> dir, int* traversals) {
    if (dir.first == 1) {
        for (int i = 0; i < this->width; ++i)
            traversals[i] = this->width - 1 - i;
    }
    else {
        for (int i = 0; i < this->width; ++i)
            traversals[i] = i;
    }
}

void Board::getYTraversals(pair<int, int> dir, int* traversals) {
    if (dir.second == 1) {
        for (int i = 0; i < this->width; ++i)
            traversals[i] = this->width - 1 - i;
    }
    else {
        for (int i = 0; i < this->width; ++i)
            traversals[i] = i;
    }
}

bool Board::coordsInBounds(pair<int, int> pos) {
    return (pos.first >= 0  && pos.first < this->width) &&
           (pos.second >= 0 && pos.second < this->width);
}

bool Board::slotOccupied(pair<int, int> pos) {
    return !this->board[pos.first][pos.second].isEmpty();
}

pair<int, int> Board::findShiftDestination(int x, int y, pair<int, int> vec) {
    pair<int, int> curr = make_pair(x, y);
    pair<int, int> dest = curr;
    do {
        curr = dest;

        dest.first  += vec.first;
        dest.second += vec.second;
    } while ( this->coordsInBounds(dest) && !(this->slotOccupied(dest)) );

    // cout << "Blocked at: " << dest.first << ", " << dest.second << endl;
    // cout << this->coordsInBounds(dest) << " - " << this->slotOccupied(dest) << endl;

    return curr;
}

pair<bool, int> Board::shift(int dir) {
    pair<int, int> vec = this->getVector(dir);

    bool someTileMoved = false;
    int score = 0;

    int xTraversals[4], yTraversals[4];

    this->getXTraversals(vec, xTraversals);
    this->getYTraversals(vec, yTraversals);

    for (int i = 0; i < this->width; ++i) {
        int x = xTraversals[i];
        for (int j = 0; j < this->width; ++j) {
            int y = yTraversals[j];

            if (this->board[x][y].isEmpty()) continue;

            this->board[x][y].setMerged(false);

            pair<int, int> farthestOpen = this->findShiftDestination(x, y, vec);
            pair<int, int> next = make_pair(farthestOpen.first + vec.first, farthestOpen.second + vec.second);


            if ( this->coordsInBounds(next) &&
                 !this->board[next.first][next.second].isMerged() &&
                 this->board[next.first][next.second] == this->board[x][y] ) {

                this->board[next.first][next.second] *= 2;
                this->board[next.first][next.second].setMerged(true);
                score += this->board[next.first][next.second];
                this->board[x][y] = 0;
                someTileMoved = true;
            }
            else if ( !(x == farthestOpen.first && y == farthestOpen.second) ) {
                this->board[farthestOpen.first][farthestOpen.second] = this->board[x][y];
                this->board[x][y] = 0;
                someTileMoved = true;
            }
        }
    }

    if (someTileMoved) {
        lastMove = dir;
    }

    return make_pair(someTileMoved, score);
}

bool Board::slotsAvailable() {
    for (int x = 0; x < this->width; ++x) {
        for (int y = 0; y < this->width; ++y) {
            if (this->board[x][y].isEmpty())
                return true;
        }
    }
    return false;
}

bool Board::matchesPossible() {
    for (int x = 0; x < this->width; ++x) {
        for (int y = 0; y < this->width; ++y) {
            if (this->board[x][y].isEmpty()) continue;

            for (int dir = 0; dir < 4; ++dir) {
                pair<int, int> vec = this->getVector(dir);
                pair<int, int> otherSlot = make_pair(x + vec.first, y + vec.second);

                if (this->coordsInBounds(otherSlot) &&
                    (this->board[x][y] == this->board[otherSlot.first][otherSlot.second]))
                    return true;
            }
        }
    }
    return false;
}

bool Board::movesAvailable() {
    return this->slotsAvailable() || this->matchesPossible();
}

Board& Board::operator=(const Board& other) {
    if (!this->initialized) this->initialize();

    for (int i = 0; i < this->width; ++i) {
        for (int j = 0; j < this->width; ++j) {
            this->board[i][j] = other.board[i][j];
        }
    }

    return *this;
}

ostream& operator<<(ostream& out, const Board& board) {
    for (int i = 0; i < board.width; i++) {
        for (int j = 0; j < board.width; j++) {
            out << setw(5) << board.board[i][j];
        }
        out << endl << endl;
    }

    return out;
}