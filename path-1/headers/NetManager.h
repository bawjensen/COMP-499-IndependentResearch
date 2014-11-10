#pragma once

#include "NeuralNet.h"

class NetManager {
private:
    NeuralNet* nets;
    int* scores;
    int numNets;

public:
    NetManager();
    NetManager(int& numNets);
    ~NetManager();

    void initialize(int& numNets);

    void keepScore(int& score, int& index);
    void mutateWinners();

    NeuralNet& operator[](int& index);
};