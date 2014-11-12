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

    bool initialized;

    float generateRand();
    float mutationValue();
    float biasMutationValue();

    void copyFrom(const NeuralNet& other);

    void destroy();
public:
    NeuralNet();
    // NeuralNet(const NeuralNet& other);
    ~NeuralNet();

    void initialize(int inputSize, int hiddenSize);

    float run(float* inputLayer);
    float activate(float value);

    void mutate();

    NeuralNet& operator=(const NeuralNet& other);

    friend std::ostream& operator<<(std::ostream& co, const NeuralNet& net);
};