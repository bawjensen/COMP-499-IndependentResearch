#include "../headers/NetManager.h"
#include "../headers/NeuralNet.h"

#include <iostream>

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
    delete[] this->scores;
}

void NetManager::initialize(int& numNets) {
    this->numNets = numNets;
    this->nets = new NeuralNet[numNets];
    this->scores = new int[numNets];
}

void NetManager::mutateWinners() {
    int k = this->numNets / 2;

    int* tempScores = new int[this->numNets];
    for (int i = 0; i < this->numNets; ++i) {
        tempScores[i] = this->scores[i];
    }

    int kthSmallest = select(tempScores, 0, this->numNets - 1, k);

    for (int i = 0; i < this->numNets; ++i) {
        if (this->scores[i] < kthSmallest) {
            // TODO: Move the nets over, and mutate the winners
        }
    }
}

void NetManager::keepScore(int& score, int& index) {
    this->scores[index] = score;
}

NeuralNet& NetManager::operator[](int& index) {
    return this->nets[index];
}