#include "../headers/RandomGen.h"

#include <random>

using namespace std;

RandomGen* RandomGen::instance = NULL;

RandomGen::RandomGen() {
}

RandomGen* RandomGen::getInstance() {
    if (instance == NULL) {
        instance = new RandomGen;
    }

    return instance;
}

float RandomGen::operator()(float stdDev) {
    return this->generate_v1(stdDev);
    // return this->generate_v2();
    // return this->generate_v3();
    // return this->generate_v4();
}

float RandomGen::generate_v1(float stdDev) {
    normal_distribution<float> distribution(0.0f, stdDev);
    return distribution(this->generator);
}

float RandomGen::generate_v2() {
    return (rand() / (float)RAND_MAX) - 0.5f;
}

float RandomGen::generate_v3() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float RandomGen::generate_v4() {
    normal_distribution<float> distribution(0.5f, 0.5f);
    return distribution(this->generator);
}