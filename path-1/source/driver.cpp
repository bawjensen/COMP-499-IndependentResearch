#include "../headers/GameController.h"

#include <iostream>

using namespace std;

bool debug;

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
                case 'n':
                    cout << "Enabling net testing mode" << endl;
                    gc.setTestingNets(true);
                    break;
            }
        }
        else {
            if ((i > 0) && (args[i-1].length() > 0) && (args[i-1][0] != '-')) {
                cout << "Ended flags, grabbing cmd line args" << endl;
                break; // Finished with flags, time to grab cmd line args
            }
        }
    }

    // if (i+3 >= args.size()) {
    //     cout << "Warning: Too few parameters, defaulting to 10 generations, 10 nets and 10 games" << endl;
    //     gc.setNumGenerations(10);
    //     gc.setNumNets(10);
    //     gc.setNumGamesPerNet(10);
    // }
    // else {
    //     gc.setNumGenerations(args[i]);
    //     gc.setNumNets(args[i+1]);
    //     gc.setNumGamesPerNet(args[i+2]);
    // }

    gc.start();

    cout << "...done." << endl;

}
