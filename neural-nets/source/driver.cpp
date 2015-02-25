#include "../headers/GameController.h"
#include "../headers/RandomGen.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>

using namespace std;

int main(int argc, char** argv) {
    // string configFileName;
    // if (argc > 2 && string(argv[1]) == "-t") {
    //     configFileName = argv[2];
    // }
    // else {
    //     configFileName = "test-configs/test.cfg";
    // }

    // cout << "Running: " << configFileName << endl;
    // TestingSuite::start(configFileName);
    // cout << "...done." << endl;

    int gens, nets, games, hSize, treeDepth;
    float randomMean, randomStdDev;
    char evalMode;
    string trainingRunLabel;

    /*
    Arguments to the executable:
    1. Label for this training set (same thing as folder name)
    2. Number of generations
    3. Number of nets per generation
    4. Number of games per net per generation
    5. Size of the hidden layer
    6. Depth of parsing for minimax tree
    7. Mean for gaussian random number
    8. Standard deviation for gaussian random number
    9. Mode for evaluating the "quality" of a net
    */
    if (argc != 10) // Number of arguments plus the executable
        throw runtime_error("Invalid number of arguments");

    istringstream convertedArgv[10];
    for (int i = 0; i < argc; ++i) {
        convertedArgv[i].str(argv[i]);
    }

    trainingRunLabel = argv[1];

    convertedArgv[2] >> gens;
    convertedArgv[3] >> nets;
    convertedArgv[4] >> games;

    convertedArgv[5] >> hSize;
    convertedArgv[6] >> treeDepth;

    convertedArgv[7] >> randomMean;
    convertedArgv[8] >> randomStdDev;

    convertedArgv[9] >> evalMode;

    GameController gc;

    if (trainingRunLabel.find(' ') != string::npos) 
        throw runtime_error("Label cannot contain spaces");

    RandomGen::initialize(randomMean, randomStdDev);
    gc.start(gens, nets, games, hSize, evalMode, treeDepth);
    
    gc.saveNetsTo("runs/" + trainingRunLabel);
}
