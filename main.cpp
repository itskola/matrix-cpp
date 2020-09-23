#include <iostream>

#include "Matrix.h"

int main(void) {
    Matrix A = Matrix("matrixA.txt");
    std::cout << "A\n"; A.print();
    
    std::cout << "--------------" << std::endl;

    Matrix B = Matrix("matrixB.txt");
    std::cout << "B\n"; B.print();

    std::cout << "==============" << std::endl;

    Matrix plus = A + B;
    std::cout << "A + B\n"; plus.print();

    std::cout << "--------------" << std::endl;

    Matrix times = A * B;
    std::cout << "A * B\n"; times.print();

    return 0;
}