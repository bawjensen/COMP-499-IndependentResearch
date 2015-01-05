#pragma once

#include <random>

enum Mode {
    MODE_ONE,
    MODE_TWO,
    MODE_THREE,
    MODE_FOUR
};

class RandomGen {
private:
    static std::default_random_engine generator;

    static Mode mode;
    static float mean, stdDev;

    static float generate_v1();
    static float generate_v2();
    static float generate_v3();
    static float generate_v4();


public:
    static void setMode(int intMode);
    static int getMode();

    static void initialize(float mean, float stdDev);

    static float generate();
};