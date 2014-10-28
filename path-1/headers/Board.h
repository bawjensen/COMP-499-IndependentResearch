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

    std::vector<int*> getAvailableCells();

    bool addPiece(int x, int y);
    void addRandomTile();
    void shiftDown();

    friend std::ostream& operator<<(std::ostream& out, Board& board);
};