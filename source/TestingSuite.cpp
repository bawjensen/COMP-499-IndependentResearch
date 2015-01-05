#include "../headers/TestingSuite.h"
#include "../headers/GameController.h"

#include <sys/stat.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

long TestingSuite::numMoves = 0;

void TestingSuite::start(string configFile) {
    GameController gc;


    ifstream inFile(configFile);

    if (!inFile.is_open()) {
        throw runtime_error("File path invalid: " + configFile);
    }

    string configSetLabel;
    inFile >> configSetLabel;

    int gens, nets, games, hSize, treeDepth;
    char evalMode;
    string configLabel;

    if (inFile.eof()) {
        throw runtime_error("Invalid file format, no configuration lines");
    }

    string stringBuffer;

    // while (inFile >> configLabel) {
    while (getline(inFile, stringBuffer)) {
        istringstream streamBuffer(stringBuffer);

        // Needed (from README) (o is incomplete, + is already a feature):
        // + Generations
        // + Nets
        // + Games
        // + Hidden Size
        // + Survivor Choice Method (which scoring system determines how 'well' a net did)
        // o Mutation Rate (random boolean to determine whether or not to mutate)
        // o Mutation Method (different distributions, different std dev's)
        // o Random insertion of newcomers (yes/no, possibly how many/how often)
        // o Cross-mutation (two parents instead of the current one)
        // + Depth of game tree
        // o Normalize or just flatten

        // Grab all the data
        if (!(streamBuffer >> configLabel >> gens >> nets >> games >> hSize >> evalMode >> treeDepth)) { throw runtime_error("Improperly formatted file"); };

        // Create new directory for the data

        string runLabel = configSetLabel + "_" + configLabel;

        cout << "Currently running: " << runLabel << endl;

        mkdir(runLabel.c_str(), 0755); // 0755 is the code for rwxr-xr-x as permissions (0 is essential)
        gc.redirectOutputTo(runLabel + "/output.csv");

        // Actually run the thing
        
        auto start = chrono::steady_clock::now();

        gc.start(gens, nets, games, hSize, evalMode, treeDepth);

        auto end = chrono::steady_clock::now();
        
        double numSec = chrono::duration<double>(end - start).count();
        cout << endl
            << "moves," << TestingSuite::numMoves << endl
            << "moves/sec," << (int)(TestingSuite::numMoves / numSec) << endl
            << "seconds," << numSec << endl
            << "minutes," << (numSec / 60.0f) << endl
            << "hours," << (numSec / 3600.0f) << endl
            << "days," << (numSec / 86400.0f) << endl;

        // Save and clean up
        
        gc.saveNetsTo(runLabel);
        gc.reset();
        gc.restoreOutput();
    }


}