#include "../headers/NeuralNet.h"

#include <iostream>

using namespace std;

NeuralNet::NeuralNet() {
    this->initialize();
}

void NeuralNet::initialize() {
    // Sizes
    this->inputSize = 16;
    this->hiddenSize = 4;
    this->numHiddenLayers = 1;

    // Edges and weights

    this->edgeWeights = new float**[this->numHiddenLayers+1];

    // Set up layers of edge weights, except the last one
    for (int n = 0; n < this->numHiddenLayers; ++n ) {
        this->edgeWeights[n] = new float*[this->hiddenSize];
        
        for (int i = 0; i < this->hiddenSize; ++i) {
            this->edgeWeights[n][i] = new float[this->inputSize];

            for (int j = 0; j < this->inputSize; ++j) {
                this->edgeWeights[n][i][j] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            }
        }
    }

    // Set up final layer of edge weights
    int n = this->numHiddenLayers;
    this->edgeWeights[n] = new float*[1];
    this->edgeWeights[n][0] = new float[this->inputSize];

    for (int j = 0; j < this->hiddenSize; ++j) {
        this->edgeWeights[n][0][j] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }

    // Hidden layer
    this->hiddenLayer = new float[this->hiddenSize];
    // for (int i = 0; i < this->hiddenSize; ++i) {
        // this->
    // }
}

float NeuralNet::run(float* inputLayer) {
    float sum;
    for (int i = 0; i < this->hiddenSize; ++i) {
        sum = 0.0f;

        for (int j = 0; j < this->inputSize; ++j) {
            sum += this->edgeWeights[0][i][j] * inputLayer[j];
        }

        this->hiddenLayer[i] = this->activate(sum);
    }

    sum = 0.0f;
    for (int j = 0; j < this->hiddenSize; ++j) {
        sum += this->edgeWeights[1][0][j] + this->hiddenLayer[j];
    }
    float output = this->activate(sum);

    delete[] inputLayer; // TODO: Figure out a better deletion system to avoid memory leaks

    return output;
}

float NeuralNet::activate(float value) {
    return value;
}