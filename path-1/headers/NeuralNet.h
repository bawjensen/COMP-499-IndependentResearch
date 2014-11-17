#pragma once

#include <iostream>

class NeuralNet {
private:
    float*** edgeWeights;
    float* hiddenLayer;
    float* hiddenBiases;

    // float output;

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

    float run(float* inputLayer) const;
    float activate(float value) const;

    void mutate();

    NeuralNet& operator=(const NeuralNet& other);

    std::string serialize();
    void deserialize(std::ifstream&);
    friend std::ostream& operator<<(std::ostream& co, const NeuralNet& net);
};