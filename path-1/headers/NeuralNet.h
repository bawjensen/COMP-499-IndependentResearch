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

    float run(float* inputLayer);
    float activate(float value);
};