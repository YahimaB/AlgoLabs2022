//
// Created by Yaroslav on 12/14/2022.
//
#include "gtest/gtest.h"
#include "sort.h"

class SortTest : public ::testing::Test {
protected:
    template<typename T, typename Compare>
    void ValidateSort(T *arr, int size, Compare comp) {
        T *actual = new T[size];
        T *expected = new T[size];
        for (int i = 0; i < size; i++) {
            actual[i] = arr[i];
            expected[i] = arr[i];
        }

        sort(actual, actual + size - 1, comp);
        std::sort(expected, expected + size);

        for (int i = 0; i < size; i++) {
            EXPECT_EQ(actual[i], expected[i]);
        }
    }
};

TEST_F(SortTest, normalOrder) {
    const int size = 20;
    int intArr[size];
    std::string strArr[size];

    for (int i = 0; i < size; i++) {
        intArr[i] = i;
        strArr[i] = "tmp" + std::to_string(i);
    }

    ValidateSort(intArr, size, [](int a, int b) { return a < b; });
    ValidateSort(strArr, size, [](const std::string& a, const std::string& b) { return a < b; });
}

TEST_F(SortTest, reverseOrder) {
    const int size = 20;
    int intArr[size];
    std::string strArr[size];

    for (int i = size - 1; i >= 0; i--) {
        intArr[i] = i;
        strArr[i] = "tmp" + std::to_string(i);
    }

    ValidateSort(intArr, size, [](int a, int b) { return a < b; });
    ValidateSort(strArr, size, [](const std::string& a, const std::string& b) { return a < b; });
}

TEST_F(SortTest, randomOrder) {
    const int size = 20;
    int intArr[size];
    std::string strArr[size];

    for (int i = 0; i < size; ++i) {
        int randInt = 0 + (rand() % static_cast<int>(32 + 1));
        intArr[i] = randInt;
        strArr[i] = "tmp" + std::to_string(randInt);
    }

    ValidateSort(intArr, size, [](int a, int b) { return a < b; });
    ValidateSort(strArr, size, [](const std::string& a, const std::string& b) { return a < b; });
}