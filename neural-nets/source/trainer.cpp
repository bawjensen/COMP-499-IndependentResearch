#include "../headers/GameController.h"
#include "../headers/RandomGen.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>

using namespace std;

const int NUM_ARGS = 9;

void saveNet(NeuralNet& net) {
    // Serialize and save net
    ofstream outFile("best.net");
    if (!outFile.is_open())
        throw new runtime_error("File to serialize best net into didn't open");
    outFile << net.serialize();
    outFile.close();
}

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
    if (argc != NUM_ARGS) // Number of arguments plus the executable
        throw runtime_error("Invalid number of arguments");

    istringstream convertedArgv[NUM_ARGS];
    for (int i = 0; i < NUM_ARGS; ++i) {
        convertedArgv[i].str(argv[i]);
    }

    convertedArgv[1] >> gens;
    convertedArgv[2] >> nets;
    convertedArgv[3] >> games;
    convertedArgv[4] >> hSize;
    convertedArgv[5] >> treeDepth;
    convertedArgv[6] >> randomMean;
    convertedArgv[7] >> randomStdDev;
    convertedArgv[8] >> evalMode;

    GameController gc;

    RandomGen::initialize(randomMean, randomStdDev);
    NeuralNet bestNet = gc.runTraining(gens, nets, games, hSize, evalMode, treeDepth);
    
    gc.saveNets(nets);
    saveNet(bestNet);
}
