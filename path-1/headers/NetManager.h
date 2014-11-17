#pragma once

#include "NeuralNet.h"

class NetManager {
private:
    NeuralNet* nets;
    std::pair<float, NeuralNet*>* scoreNetPairs;
    int numNets;

public:
    NetManager();
    NetManager(int& numNets);
    ~NetManager();

    void initialize(const int& numNets);

    void keepScore(const float& score, const int& index);
    void selectAndMutateSurvivors();

    NeuralNet& operator[](const int& index);
};