#include "../headers/GameController.h"

#include <iostream>

using namespace std;

bool debug;

int main(int argc, char** argv) {
    GameController gc;

    std::vector<std::string> args(argv, argv+argc);
    for (size_t i = 1; i < args.size(); ++i) {
        if (args[i] == "-d") {
            cout << "Enabling game controller debug mode" << endl;
            GameController::debug = true;
        }
        else if (args[i] == "-n") {
            cout << "Enabling net testing mode" << endl;
            gc.setTestingNets(true);
        }
    }

    gc.start();

    cout << "...done." << endl;

}
