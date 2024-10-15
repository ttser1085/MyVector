#include "MyVector/MyVector.h"

#include <iostream>

int main() {
    MyVector<int> vec;
    vec.reserve(100);
    std::cout << vec.capacity() << '\n';


    // TODO: add tests

    return 0;
}