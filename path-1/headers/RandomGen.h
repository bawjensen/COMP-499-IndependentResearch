#pragma once

#include <random>

class RandomGen {
private:
    static RandomGen* instance;
    RandomGen();

    std::default_random_engine generator;

    float generate_v1(float stdDev);
    float generate_v2();
    float generate_v3();
    float generate_v4();

public:
    static RandomGen* getInstance();

    float operator()(float stdDev);
};