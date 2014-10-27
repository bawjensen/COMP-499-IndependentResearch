#include "../headers/InputHandler.h"

InputHandler::InputHandler() {
}

InputHandler::Direction InputHandler::convert(char input) {
    Direction converted;

    if (input == 'w')
        converted = UP;
    else if (input == 'a')
        converted = LEFT;
    else if (input == 's')
        converted = DOWN;
    else if (input == 'd')
        converted = RIGHT;
}