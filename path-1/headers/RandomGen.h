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

    static float generate_v1(float stdDev);
    static float generate_v2();
    static float generate_v3();
    static float generate_v4();


public:
    static float generate(float stdDev);

    static void setMode(int intMode);
};