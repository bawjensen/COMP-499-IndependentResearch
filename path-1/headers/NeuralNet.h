#pragma once

class NeuralNet {
private:
    float*** edgeWeights;
    float* hiddenLayer;
    int inputSize;
    int hiddenSize;
    int numHiddenLayers;

    void initialize();
public:
    NeuralNet();
    NeuralNet(NeuralNet& other);

    float run(float* inputLayer);
    float activate(float value);

    void mutate();
};