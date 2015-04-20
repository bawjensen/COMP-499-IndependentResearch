#include "../headers/NetManager.h"
#include "../headers/NeuralNet.h"

#include <iostream>
#include <algorithm>

using namespace std;

int partition(int* arr, int left, int right, int pivotIndex) {
    int pivotVal = arr[pivotIndex];

    int temp = arr[right];
    arr[right] = arr[pivotIndex];
    arr[pivotIndex] = temp;

    int storeIndex = left;

    for (int i = left; i < right; ++i) {
        if (arr[i] < pivotVal) {
            temp = arr[storeIndex];
            arr[storeIndex] = arr[i];
            arr[i] = temp;
            ++storeIndex;
        }
    }

    temp = arr[right];
    arr[right] = arr[storeIndex];
    arr[storeIndex] = temp;

    return storeIndex;
}

int select(int* arr, int left, int right, int k) {
    if (left == right) {
        return arr[left];
    }

    int pivotIndex;
    while (pivotIndex != k) {
        pivotIndex = (left + right) / 2;
        pivotIndex = partition(arr, left, right, pivotIndex);

        if (pivotIndex > k) {
            right = pivotIndex - 1;
        }
        else {
            left = pivotIndex + 1;
        }
    }
    return arr[pivotIndex];
}

// =============================================================================

NetManager::NetManager() {

}

NetManager::NetManager(const int& numNets, const int& hiddenLayerSize) {
    this->initialize(numNets, hiddenLayerSize);
}

NetManager::~NetManager() {
    delete[] this->nets;
    delete[] this->scoreNetPairs;
}

void NetManager::initialize(const int& numNets, const int& hiddenLayerSize) {
    this->numNets = numNets;
    this->nets = new NeuralNet[numNets];
    this->scoreNetPairs = new pair<float, NeuralNet*>[numNets];

    for (int i = 0; i < this->numNets; i++) {
        this->nets[i].initialize(16, hiddenLayerSize); // 16 is size of board when flattened
        this->scoreNetPairs[i] = make_pair(0, &this->nets[i]);
    }
}

bool compare(const pair<float, NeuralNet*>& first, const pair<float, NeuralNet*>& second) {
    return first.first > second.first;
}

void NetManager::selectAndMutateSurvivors(int numParents) {
    int k = this->numNets / 2;

    // Partial sorts it, not worrying about order but moving everything less than the kth element to the front half
    // Performs a little like quicksort
    nth_element(this->scoreNetPairs, this->scoreNetPairs + k, this->scoreNetPairs + this->numNets, compare);

    for (int i = 0; i < k; ++i) {
        NeuralNet** parents = new NeuralNet*[numParents]; // Pointers to the parents of the baby net
        parents[0] = this->scoreNetPairs[i].second; // Manually choose first parent to be the current net
        int randChoice;

        for (int j = 1; j < numParents; ++j) {
            while ( ( randChoice = (rand() % k) ) == i ); // Loop until our random choice isn't our original choice
            parents[j] = this->scoreNetPairs[rand() % k].second; // Randomly choose other parents
        }

        this->scoreNetPairs[i+k].second->inheritFrom(parents, numParents);

        // Second element of pair is the net
        // Take the survivor (in the front half) and assign it into the "killed" net
        // (*this->scoreNetPairs[i+k].second) = (*this->scoreNetPairs[i].second);

        // Mutate the new copy of the net
        this->scoreNetPairs[i+k].second->mutate();

        delete[] parents;
    }
}

void NetManager::keepScore(const float& score, const int& i) {
    this->scoreNetPairs[i] = make_pair(score, &this->nets[i]);
}

NeuralNet& NetManager::operator[](const int& i) {
    return this->nets[i];
}