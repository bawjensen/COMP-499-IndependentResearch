// #include "../headers/GameController.h"
// #include "../headers/RandomGen.h"
#include "../headers/TestingSuite.h"

#include <iostream>
#include <stdexcept>
#include <unistd.h>

using namespace std;

int main(int argc, char** argv) {

    #ifdef __APPLE__
    chdir("/Users/bryanjensen/Desktop/499IR/");
    #endif

    string configFileName;
    if (argc > 2 && string(argv[1]) == "-t") {
        configFileName = argv[2];
    }
    else {
        configFileName = "test-configs/test.cfg";
    }
    cout << "Running: " << configFileName << endl;
    TestingSuite::start(configFileName);
    cout << "...done." << endl;
}