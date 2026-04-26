#include "../include/matrix.h"
#include <gtest/gtest.h>
#include <random>
#include <vector>

TEST(Matrix, test1){
    float Mdata[] = {1, 0, 0, 1};
    int Mrow = 2;
    int Mcolumn = 2;
    Matrix M(Mdata, Mrow, Mcolumn);
    auto M_inv = Matrix::getInversed(M, 100);

    int N;
    float expected_data[] = {1, 0, 0, 1};
    float* data = M_inv.getData(N);
    for(int i = 0; i < N; ++i){
        EXPECT_EQ(data[i], expected_data[i]);
    }
    free(data);
}

TEST(Matrix, test2){
    float Mdata[] = {2, 3, 1, 4, 5, 6, 7, 8, 9};
    int Mrow = 3;
    int Mcolumn = 3;
    Matrix M(Mdata, Mrow, Mcolumn);
    auto M_inv = Matrix::getInversed(M, 70000);

    int N;
    float expected_data[] = {(float)(-3)/9, (float)-(19)/9, (float)(13)/9, (float)6/9, (float)(11)/9, (float)(-8)/9, (float)(-3)/9, (float)(5)/9, (float)(-2)/9};
    float* data = M_inv.getData(N);
    for(int i = 0; i < N; ++i){
        if(data[i] > 0){
            EXPECT_TRUE(expected_data[i] - 0.01 <= data[i] && data[i] <= expected_data[i] + 0.01);
        }
        else{
            EXPECT_TRUE((expected_data[i] - 0.01 <= data[i]) && (data[i] <= expected_data[i] + 0.01));
        }
    }
    free(data);
}

void fillRandom(float* data, const int size, const float min = -10.0, const float max = 10.0) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    for (int i = 0; i < size; ++i) {
        data[i] = dist(gen);
    }
}

TEST(Matrix, RandomInverseTest) {
    const int size = 100;
    const int n = size * size;
    std::vector<float> randomData(n);

    fillRandom(randomData.data(), n, -1.0, 1.0);
    for(int i = 0; i < size; ++i) {
        randomData[i * size + i] += 10.0;
    }

    const Matrix A(randomData.data(), size, size);

    const Matrix A_inv = Matrix::getInversed(A, 10000);

    const Matrix IdentityCheck = A * A_inv;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            const float epsilon = 1e-3f;
            const float val = IdentityCheck.getElement(i, j);
            if (i == j) {
                EXPECT_NEAR(val, 1.0, epsilon);
            } else {
                EXPECT_NEAR(val, 0.0, epsilon);
            }
        }
    }
}
