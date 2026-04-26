#include "../include/matrix.h"

#include <iostream>
#include <vector>

int main(const int argc, char* argv[]) {
    int N; int M; int precision;
    std::vector<float> dataForMatrix;
    if(argc > 4){
        N = std::stoi(argv[1]);
        M = std::stoi(argv[2]);
        precision = std::stoi(argv[3]);
        for (int i = 4; i < argc; ++i) {
            dataForMatrix.push_back(std::stof(argv[i]));
        }
    }
    else{
        std::cout << "use <N> <M> <your matrix> <precision>" << std::endl;
        return 1;
    }
    Matrix A(dataForMatrix.data(), N, M);
    A = Matrix::getInversed(A, precision);
    A.showMatrix();
}