#include "../headers/NeuralNet.h"
#include "../headers/RandomGen.h"

#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;

NeuralNet::NeuralNet() {
    this->initialized = false;
}

// NeuralNet::NeuralNet(const NeuralNet& other) {
//     cout << "Called copy constructor" << endl;
//     this->copyFrom(other);
// }

NeuralNet::~NeuralNet() {
    if (this->initialized) this->destroy();
}

float NeuralNet::generateRand() {
    // return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return RandomGen::generate(0.5f);
}

void NeuralNet::initialize(int inputSize, int hiddenSize) {
    if (this->initialized) {
        throw runtime_error("Cannot reinitialize a net!");
    }
    else if (inputSize == 0 || hiddenSize == 0) {
        throw invalid_argument("Cannot create a new with a layer of size 0");
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
    if (!this->initialized) {
        throw runtime_error("Cannot destroy an uninitialized net!");
    }
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
        throw runtime_error("Trying to copy incorrectly sized nets");
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
        throw runtime_error("Cannot run an uninitialized net");
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
    // output = this->activate(output);

    delete[] inputLayer; // TODO: Figure out a better deletion system to avoid memory leaks

    return output;
}

float NeuralNet::activate(float value) const {
    // return value > 0 ? 1 : 0;
    return (float)1 / (1 + exp(-value));
}

float NeuralNet::mutationValue() {
    // return (rand() / (float)RAND_MAX) - 0.5f;
    return RandomGen::generate(0.5f);
}

float NeuralNet::biasMutationValue() {
    return RandomGen::generate(2.0f);
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

string NeuralNet::serialize() {
    ostringstream strBuffer;

    strBuffer << this->inputSize << endl;
    strBuffer << this->hiddenSize << endl;

    // strBuffer << "0:" << endl;
    for (int i = 0; i < this->inputSize; ++i) {
        // strBuffer << i << " ";
        for (int j = 0; j < this->hiddenSize; ++j) {
            strBuffer << this->edgeWeights[0][i][j] << " ";
        }
        strBuffer << endl;
    }

    // strBuffer << "0:" << endl;
    for (int i = 0; i < this->hiddenSize; ++i) {
        // strBuffer << i << " ";
        strBuffer << this->edgeWeights[1][i][0];
        strBuffer << endl;
    }
    // strBuffer << "B:" << endl;
    for (int i = 0; i < this->hiddenSize; ++i) {
        strBuffer << this->hiddenBiases[i] << " ";
    }
    strBuffer << endl;

    return strBuffer.str();
}

void NeuralNet::deserialize(ifstream& file) {
    if (this->initialized) this->destroy();

    int tempInt;
    float tempFloat;

    file >> this->inputSize;
    file >> this->hiddenSize;

    this->initialize(this->inputSize, this->hiddenSize);

    for (int i = 0; i < this->inputSize; ++i) {
        for (int j = 0; j < this->hiddenSize; ++j) {
            file >> this->edgeWeights[0][i][j];
        }
    }

    for (int i = 0; i < this->hiddenSize; ++i) {
        file >> this->edgeWeights[1][i][0];
    }

    for (int i = 0; i < this->hiddenSize; ++i) {
        file >> this->hiddenBiases[i];
    }
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