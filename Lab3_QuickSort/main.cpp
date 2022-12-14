#include <iostream>
#include <fstream>
#include <chrono>
#include "sort.h"

std::chrono::nanoseconds measureSortTime(int size, bool quickSort) {
    const int num_of_steps = 100;
    auto time = std::chrono::nanoseconds(0);

    for (int step = 0; step < num_of_steps; step++) {
        int *arr = new int[size];
        for (int i = size - 1; i >= 0; i--)
            arr[i] = 100 - i;

        auto start = std::chrono::high_resolution_clock::now();
        if (quickSort) {
            sort(arr, arr + size - 1, [](int a, int b) { return a < b; });
        } else {
            insertionSort(arr, arr + size - 1, [](int a, int b) { return a < b; });
        }
        auto end = std::chrono::high_resolution_clock::now();
        time += (end - start);

        delete[] arr;
    }

    return time;
}

int main() {
    int arr[10] = {8, 3, 2, 3, 1, 4, 1, 9, 1, 10};
    sort(arr, arr + 9, [](int a, int b) { return a < b; });
    printArray(arr, arr + 9);

    minInsertion = 0;
    std::ofstream f("measures.txt");
    f << "size" << " | " << "quickSort" << " | " << "insertionSort" << "\n";

    for (int size = 2; size <= 20; size++) {

        auto quickTime = measureSortTime(size, true);
        auto insertTime = measureSortTime(size, false);

        if (f.is_open())
            f << size << " | " << quickTime.count() << " | " << insertTime.count() << "\n";
    }

    f.close();
    return 0;
}


