#include "../headers/GameController.h"

#include <iostream>
#include <sstream>

using namespace std;

int toInt(char* c_str) {
    int val;

    istringstream convert(c_str);

    convert >> val;

    return val;
}

int main(int argc, char** argv) {
    cout << "Starting" << endl;

    if (argc != 3)
        throw runtime_error("Should have three arguments (exe, net filename, ply)");

    GameController gc;

    string filename(argv[1]);
    gc.runTesting(filename, toInt(argv[2]));
    cout << "Done" << endl;
}
