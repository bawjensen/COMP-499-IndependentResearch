// #include "../headers/GameController.h"
// #include "../headers/RandomGen.h"
#include "../headers/TestingSuite.h"

#include <iostream>
#include <stdexcept>

using namespace std;

int main(int argc, char** argv) {
    TestingSuite::start("test-configs/1.cfg");
    cout << "...done." << endl;
}