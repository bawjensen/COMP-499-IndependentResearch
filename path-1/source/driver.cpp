#include "../headers/GameController.h"

#include <iostream>

using namespace std;

bool debug;

int main(int argc, char** argv) {
    GameController gc;

    std::vector<std::string> args(argv, argv+argc);
    for (size_t i = 1; i < args.size(); ++i) {
        // if (args[i] == "-d") {
        //     cout << "Enabling game controller debug mode" << endl;
        //     GameController::debug = true;
        // }
        // else if (args[i] == "-n") {
        //     cout << "Enabling net testing mode" << endl;
        //     gc.setTestingNets(true);
        // }
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
    }

    gc.start();

    cout << "...done." << endl;

}
