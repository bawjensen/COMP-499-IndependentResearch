#include "../headers/NeuralNet.h"
#include "../headers/RandomGen.h"

#include <iostream>
#include <cmath>

using namespace std;

NeuralNet::NeuralNet() {
    this->initialized = false;
}

// NeuralNet::NeuralNet(const NeuralNet& other) {
//     cout << "Called copy constructor" << endl;
//     this->copyFrom(other);
// }

NeuralNet::~NeuralNet() {
    this->destroy();
}

float NeuralNet::generateRand() {
    // return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return (*RandomGen::getInstance())(5.0f);
}

void NeuralNet::initialize(int inputSize, int hiddenSize) {
    if (this->initialized) {
        cout << "Can't reinitialize a net!" << endl;
        exit(1);
    }
    else {
        this->initialized = true;
    }

    // Sizes
    this->inputSize = inputSize;
    this->hiddenSize = hiddenSize;

    // Edges and weights

    // Set up full array of edge weight arrays
    this->edgeWeights = new float**[2]; // Two layers, input-hidden and hidden-output 

    // Set up input layer to hidden layer edge weights
    this->edgeWeights[0] = new float*[this->inputSize];
    
    for (int i = 0; i < this->inputSize; ++i) {
        this->edgeWeights[0][i] = new float[this->hiddenSize];

        for (int j = 0; j < this->hiddenSize; ++j) {
            this->edgeWeights[0][i][j] = this->generateRand();
        }
    }

    // Set up hidden layer to output layer edge weights
    this->edgeWeights[1] = new float*[this->hiddenSize];

    for (int i = 0; i < this->hiddenSize; ++i) {
        this->edgeWeights[1][i] = new float[1]; // Only one output node
        this->edgeWeights[1][i][0] = this->generateRand();
    }

    // Hidden layer and biases
    this->hiddenLayer = new float[this->hiddenSize];
    this->hiddenBiases = new float[this->hiddenSize];

    for (int i = 0; i < this->hiddenSize; ++i) {
        this->hiddenBiases[i] = this->generateRand();
    }
}

void NeuralNet::destroy() {
    // Delete input layer to hidden layer edge weights
    for (int i = 0; i < this->inputSize; ++i) {
        delete[] this->edgeWeights[0][i];
    }
    delete[] this->edgeWeights[0];

    // Delete hidden layer to output layer edge weights
    for (int i = 0; i < this->hiddenSize; ++i) {
        delete[] this->edgeWeights[1][i]; // Deleting the single allocated node
    }
    delete[] this->edgeWeights[1];

    // Delete hidden layer and biases
    delete[] this->hiddenLayer;
    delete[] this->hiddenBiases;

    // Delete all
    delete[] this->edgeWeights;
}

void NeuralNet::copyFrom(const NeuralNet& other) {
    // Sizes
    if (this->inputSize != other.inputSize || this->hiddenSize != other.hiddenSize) {
        cout << "Trying to copy incorrectly sized nets" << endl;
        exit(1);
    }

    // Edges and weights

    // Set up full array of edge weight arrays

    // Set up input layer to hidden layer edge weights
    for (int i = 0; i < this->inputSize; ++i) {
        for (int j = 0; j < this->hiddenSize; ++j) {
            this->edgeWeights[0][i][j] = other.edgeWeights[0][i][j];
        }
    }

    // Set up hidden layer to output layer edge weights
    for (int i = 0; i < this->hiddenSize; ++i) {
        this->edgeWeights[1][i][0] = other.edgeWeights[1][i][0];
    }

    // Hidden layer and biases
    for (int i = 0; i < this->hiddenSize; ++i) {
        this->hiddenBiases[i] = other.hiddenBiases[i];
    }
}

float NeuralNet::run(float* inputLayer) const {
    if (!initialized) {
        cout << "Can't run an uninitialized net!" << endl;
        exit(1);
    }

    // Compute input layer to hidden layer values
    for (int j = 0; j < this->hiddenSize; ++j) {
        this->hiddenLayer[j] = 0.0f;
        for (int i = 0; i < this->inputSize; ++i) {
            this->hiddenLayer[j] += this->edgeWeights[0][i][j] * inputLayer[i];
        }
    }
    for (int i = 0; i < this->hiddenSize; ++i) {
        this->hiddenLayer[i] = this->activate(this->hiddenLayer[i] + this->hiddenBiases[i]);
    }

    // Compute hidden layer to output layer values
    float output = 0.0f;
    for (int i = 0; i < this->hiddenSize; ++i) {
        output += this->edgeWeights[1][i][0] * this->hiddenLayer[i];
    }
    output = this->activate(output);

    delete[] inputLayer; // TODO: Figure out a better deletion system to avoid memory leaks

    return output;
}

float NeuralNet::activate(float value) const {
    // return value > 0 ? 1 : 0;
    return (float)1 / (1 + exp(-value));
}

float NeuralNet::mutationValue() {
    // return (rand() / (float)RAND_MAX) - 0.5f;
    return (*RandomGen::getInstance())(1.0f);
}

float NeuralNet::biasMutationValue() {
    return (*RandomGen::getInstance())(1.0f);
}

void NeuralNet::mutate() {
    // Mutate full array of edge weight arrays

    // Mutate input layer to hidden layer edge weights
    for (int i = 0; i < this->inputSize; ++i) {
        for (int j = 0; j < this->hiddenSize; ++j) {
            this->edgeWeights[0][i][j] += this->mutationValue();
        }
    }

    // Mutate hidden layer to output layer edge weights
    for (int i = 0; i < this->hiddenSize; ++i) {
        this->edgeWeights[1][i][0] += this->mutationValue();
    }

    // Mutate hidden layer bias values
    for (int i = 0; i < this->hiddenSize; ++i) {
        this->hiddenBiases[i] += this->biasMutationValue();
    }
}

NeuralNet& NeuralNet::operator=(const NeuralNet& other) {
    this->copyFrom(other);
    return *this;
}

ostream& operator<<(ostream& co, const NeuralNet& net) {
    co << "Displaying net at: " << &net << endl;
    co << "Edge weights at: " << &(net.edgeWeights) << endl;

    // Edges and weights
    co << "Input to hidden: " << endl;
    for (int i = 0; i < net.inputSize; ++i) {
        for (int j = 0; j < net.hiddenSize; ++j) {
            co << net.edgeWeights[0][i][j] << " ";
        }
        co << endl;
    }

    co << "Hidden to output: ";
    for (int i = 0; i < net.hiddenSize; ++i) {
        co << net.edgeWeights[1][i][0] << " ";
    }
    co << endl;

    co << "Hidden biases: ";
    for (int i = 0; i < net.hiddenSize; ++i) {
        co << net.hiddenBiases[i] << " ";
    }
    co << endl;

    return co;
}