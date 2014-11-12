#pragma once

#include <iostream>

class NeuralNet {
private:
    float*** edgeWeights;
    float* hiddenLayer;
    float* hiddenBiases;

    float output;

    int inputSize;
    int hiddenSize;

    float generateRand();
    float mutationValue();
    float biasMutationValue();

    void initialize();
    void initializeFrom(const NeuralNet& other);

    void destroy();
public:
    NeuralNet();
    // NeuralNet(const NeuralNet& other);
    ~NeuralNet();

    float run(float* inputLayer);
    float activate(float value);

    void mutate();

    NeuralNet& operator=(const NeuralNet& other);

    friend std::ostream& operator<<(std::ostream& co, const NeuralNet& net);
};