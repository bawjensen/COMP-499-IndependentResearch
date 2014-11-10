#pragma once

#include "NeuralNet.h"

class NetManager {
private:
    NeuralNet* nets;
    std::pair<int, NeuralNet*>* scoreNetPairs;
    int numNets;

public:
    NetManager();
    NetManager(int& numNets);
    ~NetManager();

    void initialize(int& numNets);

    void keepScore(int& score, int& index);
    void selectAndMutateSurvivors();

    NeuralNet& operator[](int& index);
};