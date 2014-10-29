#include <iostream>
#include <vector>

class Board {
private:
    int width;
    int** board;
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

    bool addPiece(int x, int y);
    bool addPieceManual(int x, int y, int value);
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