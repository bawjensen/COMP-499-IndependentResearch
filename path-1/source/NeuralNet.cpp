#include "../headers/NeuralNet.h"

#include <iostream>

using namespace std;

NeuralNet::NeuralNet() {
    this->initialize();
}

NeuralNet::NeuralNet(const NeuralNet& other) {
    cout << "Called copy constructor" << endl;
    this->initializeFrom(other);
}

NeuralNet::~NeuralNet() {
    this->destroy();
}

float NeuralNet::generateRand() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

void NeuralNet::initialize() {
    // Sizes
    this->inputSize = 16;
    this->hiddenSize = 4;

    // Edges and weights

    // Set up full array of edge weight arrays
    int n = 2; // 2 sets of edge weights, input->hidden and hidden->output

    this->edgeWeights = new float**[n]; 

    // Set up input layer to hidden layer edge weights
    this->edgeWeights[0] = new float*[this->inputSize];
    
    for (int i = 0; i < this->inputSize; ++i) {
        this->edgeWeights[0][i] = new float[this->hiddenSize];

        for (int j = 0; j < this->hiddenSize; ++j) {
            this->edgeWeights[0][i][j] = this->generateRand();
        }
    }

    // Set up hidden layer to output layer edge weights
    this->edgeWeights[n-1] = new float*[1]; // Only one output node

    this->edgeWeights[n-1][0] = new float[this->hiddenSize];

    for (int i = 0; i < this->hiddenSize; ++i) {
        this->edgeWeights[n-1][0][i] = this->generateRand();
    }

    // Hidden layer and biases
    this->hiddenLayer = new float[this->hiddenSize];
    this->hiddenBiases = new float[this->hiddenSize];

    for (int i = 0; i < this->hiddenSize; ++i) {
        this->hiddenBiases[i] = this->generateRand();
    }
}

void NeuralNet::initializeFrom(const NeuralNet& other) {
    // Sizes
    this->inputSize = other.inputSize;
    this->hiddenSize = other.hiddenSize;

    // Edges and weights

    // Set up full array of edge weight arrays
    int n = 2; // 2 sets of edge weights, input->hidden and hidden->output

    this->edgeWeights = new float**[n]; 

    // Set up input layer to hidden layer edge weights
    this->edgeWeights[0] = new float*[this->inputSize];
    
    for (int i = 0; i < this->inputSize; ++i) {
        this->edgeWeights[0][i] = new float[this->hiddenSize];

        for (int j = 0; j < this->hiddenSize; ++j) {
            this->edgeWeights[0][i][j] = other.edgeWeights[0][i][j];
        }
    }

    // Set up hidden layer to output layer edge weights
    this->edgeWeights[n-1] = new float*[1]; // Only one output node

    this->edgeWeights[n-1][0] = new float[this->hiddenSize];

    for (int i = 0; i < this->hiddenSize; ++i) {
        this->edgeWeights[n-1][0][i] = other.edgeWeights[n-1][0][i];
    }

    // Hidden layer and biases
    this->hiddenLayer = new float[this->hiddenSize];
    this->hiddenBiases = new float[this->hiddenSize];

    for (int i = 0; i < this->hiddenSize; ++i) {
        this->hiddenBiases[i] = other.hiddenBiases[i];
    }
}

void NeuralNet::destroy() {
    int n = 2; // 2 sets of edge weights, input->hidden and hidden->output

    // Delete input layer to hidden layer edge weights
    for (int i = 0; i < this->inputSize; ++i) {
        delete[] this->edgeWeights[0][i];
    }
    delete[] this->edgeWeights[0];

    // Delete hidden layer to output layer edge weights
    delete[] this->edgeWeights[n-1][0];
    delete[] this->edgeWeights[n-1];

    // Delete hidden layer and biases
    delete[] this->hiddenLayer;
    delete[] this->hiddenBiases;

    // Delete all
    delete[] this->edgeWeights;
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

float NeuralNet::mutationValue() {
    return (rand() / (float)RAND_MAX) - 0.5f;
}

float NeuralNet::biasMutationValue() {
    return this->mutationValue();
}

void NeuralNet::mutate() {
    // Mutate full array of edge weight arrays
    int n = 2; // 2 sets of edge weights, input->hidden and hidden->output

    // Mutate input layer to hidden layer edge weights
    for (int i = 0; i < this->inputSize; ++i) {
        for (int j = 0; j < this->hiddenSize; ++j) {
            this->edgeWeights[0][i][j] += this->mutationValue();
        }
    }

    // Mutate hidden layer to output layer edge weights
    for (int i = 0; i < this->hiddenSize; ++i) {
        this->edgeWeights[n-1][0][i] = this->mutationValue();
    }

    // Mutate hidden layer bias values
    for (int i = 0; i < this->hiddenSize; ++i) {
        this->hiddenBiases[i] = this->biasMutationValue();
    }
}

NeuralNet& NeuralNet::operator=(const NeuralNet& other) {
    this->initializeFrom(other);
    return *this;
}

ostream& operator<<(ostream& co, const NeuralNet& net) {
    co << "Displaying net at: " << &net << endl;
    co << "Edge weights at: " << &(net.edgeWeights) << endl;

    int n = 2; // 2 sets of edge weights, input->hidden and hidden->output

    // Edges and weights
    co << "Input to hidden: " << endl;
    for (int i = 0; i < net.inputSize; ++i) {
        for (int j = 0; j < net.hiddenSize; ++j) {
            co << net.edgeWeights[0][i][j];
        }
        co << endl;
    }

    co << "Hidden to output: ";
    for (int i = 0; i < net.hiddenSize; ++i) {
        co << net.edgeWeights[n-1][0][i];
    }
    co << endl;

    co << "Hidden biases: ";
    for (int i = 0; i < net.hiddenSize; ++i) {
        co << net.hiddenBiases[i];
    }
    co << endl;

    return co;
}