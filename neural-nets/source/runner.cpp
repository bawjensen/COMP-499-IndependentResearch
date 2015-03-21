#include "../headers/GameController.h"

#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    cout << "Starting" << endl;
    GameController gc;

    gc.runTesting();
    cout << "Done" << endl;
}
