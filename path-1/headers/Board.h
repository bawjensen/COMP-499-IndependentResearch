#include <iostream>
#include <vector>

class Tile {
private:
    bool merged;
    int value;
public:
    Tile() { this->value = 0; this->merged = false; };
    Tile(int value) { this->value = value; this->merged = false; };

    bool isEmpty() { return this->value == 0; };
    bool isMerged() { return this->merged; };
    void setMerged(bool merged) { this->merged = merged; };

    bool operator==(Tile& other) { return this->value == other.value; };
    Tile& operator *=(int magnitude) { this->value *= magnitude; return *this; };

    friend std::ostream& operator<<(std::ostream& co, Tile& tile) { co << tile.value; return co; };
    friend int& operator+=(int& value, Tile& tile) { value += tile.value; return value; };
};

class Board {
private:
    int width;
    Tile** board;
    static const int numPossiblePieces = 2;
    int possiblePieces[numPossiblePieces];

public:
    Board();
    Board(int width);

    void setWidth(int width) { this->width = width; };
    int getWidth() { return this->width; };

    void initialize();
    void reset();

    std::vector<int*> getAvailableCells();

    void addPiece(int x, int y);
    void addPieceManual(int x, int y, int value);
    void addRandomTile();

    std::pair<bool, int> shift(int dir);
    std::pair<int, int> findShiftDestination(int x, int y, std::pair<int, int> vec);

    std::pair<int, int> getVector(int dir);
    void getXTraversals(std::pair<int, int> vec, int* traversals);
    void getYTraversals(std::pair<int, int> vec, int* traversals);
    bool coordsInBounds(std::pair<int, int> vec);
    bool slotOccupied(std::pair<int, int> vec);

    bool slotsAvailable();
    bool matchesPossible();

    friend std::ostream& operator<<(std::ostream& out, Board& board);
};