#include "../headers/InputHandler.h"

int InputHandler::convert(char input) {
    int converted;

    switch (input) {
        case 'w':
            converted = 0;
            break;
        case 'a':
            converted = 1;
            break;
        case 's':
            converted = 2;
            break;
        case 'd':
            converted = 3;
            break;
        default:
            converted = -1;
            break;
    }

    return converted;
}