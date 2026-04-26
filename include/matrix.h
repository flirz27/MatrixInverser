
#ifndef MATRIXINVERSION_MATRIX_H
#define MATRIXINVERSION_MATRIX_H

#include <xmmintrin.h>

#define v4fSize 4
typedef __m128 v4f;

class Matrix {
public:
    Matrix(const float *data, int row, int column);
    Matrix(const Matrix& other);
    ~Matrix();

    void showMatrix() const;
    static void printMatrix(const Matrix& A);

    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    int operator==(const Matrix& other) const ;
    int operator!=(const Matrix& other) const;
    Matrix& operator=(const Matrix& other);

    void add(const Matrix& other);
    void sub(const Matrix& other);
    void mul(const Matrix& other);
    Matrix& div(float divisor);

    float getElement(int s, int n) const;
    float* getData(int& N) const;
    static Matrix getTransposed(const Matrix&);
    static Matrix getInversed(const Matrix& A, int precision);
    static Matrix getIdentityMatrix(int n);

private:
    float* data = nullptr;
    int row = 0;
    int  column = 0;
};


#endif
