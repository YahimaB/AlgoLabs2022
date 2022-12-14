#include <iostream>
#include "sort.h"

int minInsertion = 5;

int main() {
    std::cout << "Hello, World!" << std::endl;

    int arr[10] = {8, 3, 2, 3, 1, 4, 1, 9, 1, 10 };

    sort(arr, arr + 9, [](int a, int b) { return a < b; });

    printArray(arr, arr + 9);

    return 0;
}


