#include "../headers/NetManager.h"
#include "../headers/NeuralNet.h"

#include <iostream>
#include <algorithm>

using namespace std;

void printArray(int* arr, int left, int right) {
    for (int i = left; i <= right; ++i) {
        cout << arr[i] << ", ";
    }
}

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

NetManager::NetManager() {

}

NetManager::NetManager(int& numNets) {
    this->initialize(numNets);
}

NetManager::~NetManager() {
    delete[] this->nets;
    delete[] this->scoreNetPairs;
}

void NetManager::initialize(int& numNets) {
    this->numNets = numNets;
    this->nets = new NeuralNet[numNets];
    this->scoreNetPairs = new pair<int, NeuralNet*>[numNets];

    for (int i = 0; i < this->numNets; i++) {
        this->scoreNetPairs[i] = make_pair(0, &this->nets[i]);
    }
}

bool compare(const pair<int, NeuralNet*>& first, const pair<int, NeuralNet*>& second) {
    return first.first > second.first;
}

void NetManager::selectAndMutateSurvivors() {
    int k = this->numNets / 2;

    // Partial sorts it, not worrying about order but keeping everything less than the kth element to the first half
    // Performs a little like quicksort
    nth_element(this->scoreNetPairs, this->scoreNetPairs + k, this->scoreNetPairs + this->numNets, compare);

    for (int i = 0; i < k; ++i) {
        // Second element of pairs is the net
        (*this->scoreNetPairs[i+k].second) = (*this->scoreNetPairs[i].second);

        // Mutate one copy of the net
        this->scoreNetPairs[i].second->mutate();
    }
}

void NetManager::keepScore(int& score, int& i) {
    this->scoreNetPairs[i] = make_pair(score, &this->nets[i]);
}

NeuralNet& NetManager::operator[](int& i) {
    return this->nets[i];
}