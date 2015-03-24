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

    string netFileName;
    int ply = 2,
        runs = 100;

    if (argc < 2 || argc > 4)
        throw runtime_error("Should have 2-4 arguments (exe, net filename, [ply, runs])");
    
    if (argc >= 2)
        netFileName = argv[1];
    if (argc >= 3)
        ply = toInt(argv[2]);
    if (argc >= 4)
        runs = toInt(argv[3]);

    GameController gc;

    gc.runTesting(netFileName, ply, runs);
    cout << "Done" << endl;
}
