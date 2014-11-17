#include "../headers/GameController.h"

#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    // Board board(4);
    GameController gc;

    // board.addPiece();

    // cout << ""
    // for (int i = 0; i < argc; ++i) {
        // cout << argv[i] << endl;
    // }

    if (argc > 1 && string(argv[1]) == "-d") {
        gc.testNetByName("0.net");
    }
    else {
        gc.start();
    }


    // for (int i = 0; i < 5; i++) {
        // board.addRandomTile();

    // }
    cout << "...done." << endl;

}