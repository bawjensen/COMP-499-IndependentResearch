// #include "../headers/GameController.h"
// #include "../headers/RandomGen.h"
#include "../headers/TestingSuite.h"

#include <iostream>
#include <stdexcept>

using namespace std;

int main(int argc, char** argv) {
    string configFileName;
    if (argc > 2 && string(argv[1]) == "-t") {
        configFileName = argv[2];
    }
    else {
        configFileName = "test-configs/1.cfg";
    }
    cout << "Running: " << configFileName << endl;
    TestingSuite::start(configFileName);
    cout << "...done." << endl;
}