#include "../include/matrix.h"

#include <cstring>
#include <iostream>
#include <xmmintrin.h>

Matrix::Matrix(const float *data, const int row, const int column){
    if(data == nullptr){
        throw std::invalid_argument("wrong data ptr for matrix constructor");
    }
    if(row < 0){
        throw std::invalid_argument("wrong row for matrix constructor");
    }
    if(column < 0){
        throw std::invalid_argument("wrong column for matrix constructor");
    }

    auto* newData = static_cast<float *>(calloc(row * column, sizeof(float)));
    if(newData == nullptr){
        throw std::bad_alloc();
    }
    std::memcpy(newData, data, row*column*sizeof(float));
    this->data = newData;
    this->row = row;
    this->column = column;
}

Matrix::Matrix(const Matrix &other) {
    row = other.row;
    column = other.column;
    auto* newData = static_cast<float *>(malloc(row * column * sizeof(float)));
    if(newData == nullptr){
        throw std::bad_alloc();
    }
    std::memcpy(newData, other.data, row*column*sizeof(float));
    this->data = newData;
}

Matrix::~Matrix() {
    free(this->data);
}

void Matrix::showMatrix() const {
    for (int i = 0; i < this->column*this->row; ++i) {
        std::cout << *(this->data + i) << " ";
        if ((i+1)%this->column == 0 && i!=0) {
            std::cout << "\n";
        }
    }
}

void Matrix::printMatrix(const Matrix &A) {
    for(int i = 0; i < A.row; ++i){
        for(int j = 0; j < A.column; ++j){
            float a_ij = A.getElement(i, j);
            std::cout << a_ij << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

Matrix Matrix::operator+(const Matrix& other) const{
    if (row != other.row || column != other.column) {
        std::cerr << "both matrix must have the same size" << std::endl;
        throw std::invalid_argument("both matrix must have the same size");
    }

    int n = row * column;
    auto *resData = static_cast<float *>(calloc(n, sizeof(float)));
    if (!resData) {
        throw std::bad_alloc();
    }

    int i = 0;
    for (; i + 4 <= n; i += 4) {
        __m128 a = _mm_loadu_ps(&data[i]);
        __m128 b = _mm_loadu_ps(&other.data[i]);
        __m128 r = _mm_add_ps(a, b);
        _mm_storeu_ps(&resData[i], r);
    }

    for (; i < n; ++i) {
        resData[i] = data[i] + other.data[i];
    }

    Matrix res(resData, row, column);
    free(resData);
    return res;
}


Matrix Matrix::operator-(const Matrix& other) const {
    if (row != other.row || column != other.column) {
        throw std::invalid_argument("both matrix must have the same size");
    }

    int n = row * column;
    auto* resData = static_cast<float *>(calloc(n, sizeof(float)));
    if (!resData) {
        throw std::bad_alloc();
    }

    int i = 0;
    for (; i + 4 <= n; i += 4) {
        __m128 a = _mm_loadu_ps(&data[i]);
        __m128 b = _mm_loadu_ps(&other.data[i]);
        __m128 r = _mm_sub_ps(a, b);
        _mm_storeu_ps(&resData[i], r);
    }

    for (; i < n; ++i) {
        resData[i] = data[i] - other.data[i];
    }

    Matrix res(resData, row, column);
    free(resData);
    return res;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (this->column != other.row) {
        throw std::invalid_argument("for multiply 1st matrix most have the num of column which is equal to 2nd matrix num of row");
    }
    int M = this->row;
    int K = this->column;
    int N = other.column;

    Matrix tOther = getTransposed(other);

    auto* resData = static_cast<float *>(calloc(M * N, sizeof(float)));
    if (!resData) {
        throw std::bad_alloc();
    }
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            __m128 sum = _mm_setzero_ps();
            int k = 0;

            for (; k + v4fSize <= K; k += v4fSize) {
                __m128 a = _mm_loadu_ps(&this->data[i * K + k]);
                __m128 b = _mm_loadu_ps(&tOther.data[j * K + k]);

                sum = _mm_add_ps(sum, _mm_mul_ps(a, b));
            }

            float tmp[v4fSize];
            _mm_storeu_ps(tmp, sum);
            float val = tmp[0] + tmp[1] + tmp[2] + tmp[3];

            for (; k < K; ++k) {
                val += this->data[i * K + k] * tOther.data[j * K + k];
            }

            resData[i * N + j] = val;
        }
    }

    Matrix res(resData, M, N);
    free(resData);
    return res;
}

int Matrix::operator==(const Matrix& other) const{
    if((this->row != other.row) || (this->column != other.column)){
        return 0;
    }
    for(int i = 0; i < this->row; ++i){
        for(int j = 0; j < this->column; ++j){
            if(this->getElement(i, j) != other.getElement(i, j)){
                return 0;
            }
        }
    }

    return 1;
}

int Matrix::operator!=(const Matrix& other) const {
    return !(*this == other);
}

Matrix& Matrix::operator=(const Matrix& other) {
    if(&other == this){
        return *this;
    }
    this->row = other.row;
    this->column = other.column;
    auto* newData = static_cast<float *>(calloc(this->row * this->column, sizeof(float)));
    if(newData == nullptr){
        throw std::bad_alloc();
    }
    free(this->data);
    std::memcpy(newData, other.data, row*column*sizeof(float));
    this->data = newData;
    return *this;
}

void Matrix::add(const Matrix& other){
    if (row != other.row || column != other.column) {
        throw std::invalid_argument("both matrix must have the same size");
    }

    int n = row * column;

    int i = 0;
    for (; i + 4 <= n; i += 4) {
        __m128 a = _mm_loadu_ps(&data[i]);
        __m128 b = _mm_loadu_ps(&other.data[i]);
        __m128 r = _mm_add_ps(a, b);
        _mm_storeu_ps(&this->data[i], r);
    }

    for (; i < n; ++i) {
        this->data[i] = data[i] + other.data[i];
    }
}

void Matrix::sub(const Matrix& other){
    if (row != other.row || column != other.column) {
        throw std::invalid_argument("both matrix must have the same size");
    }

    int n = row * column;

    int i = 0;
    for (; i + 4 <= n; i += 4) {
        __m128 a = _mm_loadu_ps(&data[i]);
        __m128 b = _mm_loadu_ps(&other.data[i]);
        __m128 r = _mm_sub_ps(a, b);
        _mm_storeu_ps(&this->data[i], r);
    }

    for (; i < n; ++i) {
        this->data[i] = data[i] - other.data[i];
    }
}

void Matrix::mul(const Matrix& other){
    if (this->column != other.row) {
        throw std::invalid_argument("for multiply 1st matrix most have the num of column which is equal to 2nd matrix num of row");
    }

    const auto tOther = getTransposed(other);

    auto* buf = static_cast<float *>(calloc(this->row * other.column, sizeof(float)));
    if (buf == nullptr) {
        throw std::bad_alloc();
    }

    const int M = this->row;
    const int K = this->column;
    const int N = other.column;

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            v4f sum = _mm_setzero_ps();
            int k = 0;
            for (; k + v4fSize <= K; k += v4fSize) {
                const v4f a = _mm_loadu_ps(&this->data[i * K + k]);
                const v4f b = _mm_loadu_ps(&tOther.data[j * K + k]);

                sum = _mm_add_ps(sum, _mm_mul_ps(a, b));
            }

            float tmp[v4fSize];
            _mm_storeu_ps(tmp, sum);
            float val = tmp[0] + tmp[1] + tmp[2] + tmp[3];
            for (; k < K; ++k) {
                val += this->data[i * K + k] * tOther.data[j * K + k];
            }
            buf[i * N + j] = val;
        }
    }
    free(this->data);
    this->data = buf;
    this->row = M;
    this->column = N;
    memcpy(this->data, buf, M * N * sizeof(float));
}

Matrix& Matrix::div(const float divisor) {
    for(int i = 0; i < row; ++i){
        for(int j=0; j < column; ++j){
            data[column*i + j] = data[column*i + j] / divisor;
        }
    }
    return *this;
}

float Matrix::getElement(int s, int n) const {
    return data[column * s + n];
}

float *Matrix::getData(int &N) const {
    N = row*column;
    auto* res = static_cast<float *>(calloc(N, sizeof(float)));
    if(res == nullptr){
        throw std::bad_alloc();
    }
    memcpy(res, data, N*sizeof(float));
    return res;
}

Matrix Matrix::getTransposed(const Matrix& A){
    const int resRow = A.column;
    const int resColumn = A.row;
    auto* resData = static_cast<float *>(malloc(resRow * resColumn * sizeof(float)));
    if(resData == nullptr){
        throw std::bad_alloc();
    }

    for(int i = 0; i < A.row; ++i){
        for(int j = 0; j < A.column; ++j){
            resData[j*resColumn + i] = A.data[i*A.column + j];
        }
    }
    Matrix res(resData, resRow, resColumn);
    free(resData);
    return res;
}

Matrix Matrix::getInversed(const Matrix& A, const int precision) {
    float A1 = 0;
    float A_inf = 0;
    for(int j = 0; j < A.column; ++j){
        float colSum = 0;
        for(int i = 0; i < A.row; ++i){
            float a_ij = A.getElement(i, j);
            if(a_ij < 0) a_ij = -a_ij;
            colSum += a_ij;
        }
        if(colSum > A1) A1 = colSum;
    }
    for(int i = 0; i < A.row; ++i){
        float rowSum = 0;
        for(int j = 0; j < A.column; ++j){
            float a_ij = A.getElement(i, j);
            if(a_ij < 0) a_ij = -a_ij;
            rowSum += a_ij;
        }
        if(rowSum > A_inf) A_inf = rowSum;
    }

    Matrix B(getTransposed(A));
    B.div(A1*A_inf);

    Matrix I(getIdentityMatrix(A.row));

    Matrix AT = getTransposed(A);

    Matrix BA(B);
    BA.mul(A);

    Matrix R = I;
    R.sub(BA);
    Matrix A_inversed(I);
    int pow = 1;
    Matrix poweredR(R);

    Matrix RT = getTransposed(R);
    while(pow <= precision){
        A_inversed.add(poweredR);
        poweredR.mul(R);
        ++pow;
    }
    Matrix BT = Matrix::getTransposed(B);
    A_inversed.mul(B);

    return A_inversed;
}

Matrix Matrix::getIdentityMatrix(int n) {
    if(n < 1){
        throw std::invalid_argument("n must be more than 0");
    }
    int resRow = n;
    int resColumn = n;
    auto* resData = static_cast<float *>(calloc(resRow * resColumn, sizeof(float)));
    if(resData == nullptr){
        throw std::bad_alloc();
    }
    for(int i = 0; i < n; ++i){
        resData[i*n + i] = 1;
    }
    Matrix res(resData, resRow, resColumn);
    free(resData);
    return res;
}


