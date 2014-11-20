#include "../headers/GameController.h"

#include <iostream>

using namespace std;

bool debug;

int toInt(string input) {
    return stoi(input);
}

int main(int argc, char** argv) {
    // Initialize game controller
    GameController gc;

    // Default values
    gc.setNumGenerations(10);
    gc.setNumNets(10);
    gc.setNumGamesPerNet(10);

    // Create vector from argv
    std::vector<std::string> args(argv, argv+argc);

    // Loop for flags
    for (size_t i = 1; i < args.size(); ++i) {
        if (args[i].length() > 1 && args[i][0] == '-') {
            switch (args[i][1]) {
                case 'd':
                    cout << "Enabling game controller debug mode" << endl;
                    GameController::debug = true;
                    break;
                case 't':
                    cout << "Enabling net testing mode" << endl;
                    gc.setTestingNets(true);
                    break;
                case 'g':
                    if (i+1 < args.size())
                        gc.setNumGenerations(toInt(args[i+1]));
                    else
                        throw invalid_argument("Incorrect formatting of cmd line args");
                    break;
                case 'n':
                    if (i+1 < args.size())
                        gc.setNumNets(toInt(args[i+1]));
                    else
                        throw invalid_argument("Incorrect formatting of cmd line args");
                    break;
                case 'p':
                    if (i+1 < args.size())
                        gc.setNumGamesPerNet(toInt(args[i+1]));
                    else
                        throw invalid_argument("Incorrect formatting of cmd line args");
                    break;
            }
        }
    }

    gc.start();

    cout << "...done." << endl;

}
