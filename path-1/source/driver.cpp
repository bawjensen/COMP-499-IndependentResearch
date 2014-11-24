#include "../headers/GameController.h"
#include "../headers/RandomGen.h"

#include <iostream>
#include <stdexcept>

using namespace std;

bool debug;

int toInt(string input) {
    return stoi(input);
}

void printHelpMessage() {
    cout << "Usage: <exe> [flag value]*" << endl;
    cout << "Possible flags:" << endl;
    cout << "\t-h: Help (this screen)." << endl;
    cout << endl;
    cout << "\t-d: Turns on debug (mostly just allows a lot of stdout)." << endl;
    cout << "\t-t: Testing mode - runs whatever test is set up, instead of net training." << endl;
    cout << endl;
    cout << "\t-g: How many generations of training to run." << endl;
    cout << "\t-n: How many nets per generation." << endl;
    cout << "\t-p: How many games per net." << endl;
    cout << "\t-e: Changes how the nets are evaluated ([a]verage or [t]op score)." << endl;
    cout << endl;
    cout << "\t-l: How many nodes in the hidden layer." << endl;
    cout << "\t-r: Which mode to put the random generator in." << endl;
}

int main(int argc, char** argv) {
    // Initialize game controller
    GameController gc;

    cout << "Starting with defaults of 1000 generations, 100 nets, 10 games, 16 in hidden, and evaluation: highest" << endl;

    // Default values
    gc.setNumGenerations(1000);
    gc.setNumNets(100);
    gc.setNumGamesPerNet(10);
    gc.setNetHiddenLayerSize(16);
    gc.setEvaluationMode('h');

    // Create vector from argv
    std::vector<std::string> args(argv, argv+argc);

    // Loop for flags
    for (size_t i = 1; i < args.size(); ++i) {
        if (args[i].length() > 1 && args[i][0] == '-') {
            switch (args[i][1]) {
                case 'h':
                    printHelpMessage();
                    exit(0);

                case 'd':
                    cout << "Enabling game controller debug mode" << endl;
                    GameController::debug = true;
                    break;

                case 't':
                    cout << "Enabling net testing mode" << endl;
                    gc.setTestingNets(true);
                    break;

                case 'g':
                    if (i+1 < args.size()) gc.setNumGenerations(toInt(args[i+1]));
                    else throw invalid_argument("Incorrect formatting of cmd line args");
                    break;

                case 'n':
                    if (i+1 < args.size()) gc.setNumNets(toInt(args[i+1]));
                    else throw invalid_argument("Incorrect formatting of cmd line args");
                    break;

                case 'p':
                    if (i+1 < args.size()) gc.setNumGamesPerNet(toInt(args[i+1]));
                    else throw invalid_argument("Incorrect formatting of cmd line args");
                    break;

                case 'l':
                    if (i+1 < args.size()) gc.setNetHiddenLayerSize(toInt(args[i+1]));
                    else throw invalid_argument("Incorrect formatting of cmd line args");

                case 'r':
                    if (i+1 < args.size()) RandomGen::setMode(toInt(args[i+1]));
                    else throw invalid_argument("Incorrect formatting of cmd line args");
            }
        }
    }

    gc.start();

    cout << "...done." << endl;

}
