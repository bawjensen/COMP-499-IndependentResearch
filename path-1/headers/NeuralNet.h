#pragma once

#include <iostream>

class NeuralNet {
private:
    float*** edgeWeights;
    float* hiddenLayer;
    int inputSize;
    int hiddenSize;
    int numHiddenLayers;

    void initialize();
    void initializeFrom(const NeuralNet& other);

    void destroy();
public:
    NeuralNet();
    NeuralNet(const NeuralNet& other);
    ~NeuralNet();

    float run(float* inputLayer);
    float activate(float value);

    void mutate();

    NeuralNet& operator=(const NeuralNet& other);

    friend std::ostream& operator<<(std::ostream& co, const NeuralNet& net);
};