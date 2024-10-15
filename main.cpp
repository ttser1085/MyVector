#include "MyVector/MyVector.h"

#include <iostream>
#include <assert.h>

int main() {
    // Constructor test:
    {
        MyVector<int> v;
        assert(v.size() == 0);
        assert(v.capacity() == 0);

        v.push_back(10);
        assert(v.size() == 1);
        assert(v[0] == 10);

        v.push_back(20);
        assert(v.size() == 2);
        assert(v[1] == 20);
    }

    {
        MyVector<int> v;
        v.push_back(10);
        v.push_back(20);

        MyVector<int> v_copy(v);
        assert(v_copy.size() == v.size());
        assert(v_copy[0] == 10);
        assert(v_copy[1] == 20);
    }

    {
        MyVector<int> original{10, 20, 30};
        MyVector<int> moved_vector(std::move(original));

        assert(moved_vector.size() == 3);
        assert(moved_vector[0] == 10);
        assert(moved_vector[1] == 20);
        assert(moved_vector[2] == 30);
    }

    // Assigment tests:

    {
        MyVector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);

        MyVector<int> v2;
        v2 = v;
        assert(v2.size() == 3);
        assert(v2[0] == 1);
        assert(v2[1] == 2);
        assert(v2[2] == 3);
    }

    {
        MyVector<int> v;
        v.push_back(1);
        v.push_back(2);

        MyVector<int> v2;
        v2 = std::move(v);
        assert(v2.size() == 2);
        assert(v2[0] == 1);
        assert(v2[1] == 2);

        assert(v.size() == 0);
    }

    // Other tests:

    {
        MyVector<int> v;
        v.push_back(1);
        v.push_back(2);

        v.resize(5);
        assert(v.size() == 5);
        assert(v[0] == 1);
        assert(v[1] == 2);
        assert(v[2] == 0);

        v.resize(1);
        assert(v.size() == 1);
        assert(v.capacity() >= 5);
    }

    {
        MyVector<int> v{1, 2, 3};

        v.erase(v.begin() + 1);
        assert(v.size() == 2);
        assert(v[0] == 1);
        assert(v[1] == 3);
    }

    {
        MyVector<int> v = {1, 3};

        v.insert(v.begin() + 1, 2);
        assert(v.size() == 3);
        assert(v[1] == 2);
    }

    {
        MyVector<int> v = {1, 2, 3};

        v.clear();
        assert(v.size() == 0);
        assert(v.capacity() > 0);
        assert(v == MyVector<int>());
    }

    std::cout << "All tests passed\n";

    return 0;
}