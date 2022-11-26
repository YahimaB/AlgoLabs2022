//
// Created by Yaroslav on 11/25/2022.
//
#include "gtest/gtest.h"
#include "Array.h"

class ArrayTest : public ::testing::Test {
protected:

    template<typename T>
    void DefaultConstructor() {
        Array<T> a;
        EXPECT_EQ(a.size(), 0);
        EXPECT_EQ(a.capacity(), a.DEFAULT_CAPACITY);
    }

    template<typename T>
    void CapacityConstructor(int capacity) {
        Array<T> a(capacity);
        EXPECT_EQ(a.size(), 0);
        EXPECT_EQ(a.capacity(), capacity);
    }

    template<typename T>
    void ValidateInsert(T *items, int size) {
        Array<T> a;

        for (int i = 0; i < size; i++) {
            a.insert(items[i]);
            EXPECT_EQ(a[i], items[i]);
        }

        EXPECT_EQ(a.size(), size);
    }

    template<typename T>
    void ValidateInsertWithIndex(T *items, int size, T value, int index) {
        Array<T> a;
        for (int i = 0; i < size; i++) {
            a.insert(items[i]);
        }

        a.insert(index, value);
        EXPECT_EQ(a.size(), size + 1);

        for (int i = 0; i < index; i++) {
            EXPECT_EQ(a[i], items[i]);
        }

        EXPECT_EQ(a[index], value);

        for (int i = index + 1; i < a.size(); i++) {
            EXPECT_EQ(a[i], items[i - 1]);
        }
    }

    template<typename T>
    void ValidateRemove(T *items, int size, int index) {
        Array<T> a;
        for (int i = 0; i < size; i++) {
            a.insert(items[i]);
        }

        a.remove(index);
        EXPECT_EQ(a.size(), size - 1);

        for (int i = 0; i < index; i++) {
            EXPECT_EQ(a[i], items[i]);
        }

        for (int i = index; i < a.size(); i++) {
            EXPECT_EQ(a[i], items[i + 1]);
        }
    }

    template<typename T>
    void ValidateCopy(T *items, int size) {
        Array<T> a;
        for (int i = 0; i < size; i++) {
            a.insert(items[i]);
        }
        Array<T> b1 = a;
        Array<T> b2{};
        b2 = a;

        EXPECT_EQ(a.size(), b1.size());
        EXPECT_EQ(a.size(), b2.size());

        EXPECT_EQ(a.capacity(), b1.capacity());
        EXPECT_EQ(a.capacity(), b2.capacity());

        for (int i = 0; i < a.size(); i++) {
            EXPECT_EQ(a[i], b1[i]);
            EXPECT_EQ(a[i], b2[i]);
        }
    }

    template<typename T>
    void ValidateMove(T *items, int size) {
        Array<T> a, a1, a2;
        for (int i = 0; i < size; i++) {
            a.insert(items[i]);
            a1.insert(items[i]);
            a2.insert(items[i]);
        }
        Array<T> b1 = std::move(a1);
        Array<T> b2{};
        b2 = std::move(a2);

        EXPECT_EQ(a.size(), b1.size());
        EXPECT_EQ(a.size(), b2.size());

        EXPECT_EQ(a.capacity(), b1.capacity());
        EXPECT_EQ(a.capacity(), b2.capacity());

        for (int i = 0; i < a.size(); i++) {
            EXPECT_EQ(a[i], b1[i]);
            EXPECT_EQ(a[i], b2[i]);
        }
    }

    template<typename T>
    void ValidateIterator(T *items, int size) {
        Array<T> a;
        for (int i = 0; i < size; i++) {
            a.insert(items[i]);
        }

        int index = 0;
        auto it = a.iterator();
        for (; it.hasNext(); it.next()) {
            EXPECT_EQ(a[index], it.get());
            index++;
        }

        EXPECT_EQ(index, a.size());
        EXPECT_EQ(it.hasNext(), false);
    }

    template<typename T>
    void ValidateReverseIterator(T *items, int size) {
        Array<T> a;
        for (int i = 0; i < size; ++i) {
            a.insert(items[i]);
        }

        int index = size;
        auto it = a.reverseIterator();
        for (; it.hasNext(); it.next()) {
            index--;
            EXPECT_EQ(a[index], it.get());
        }

        EXPECT_EQ(index, 0);
        EXPECT_EQ(it.hasNext(), false);
    }
};

TEST_F(ArrayTest, DefaultConstructor) {
    DefaultConstructor<int>();
    DefaultConstructor<std::string>();
}

TEST_F(ArrayTest, CapacityConstructor) {
    CapacityConstructor<int>(5);
    CapacityConstructor<std::string>(5);
}

TEST_F(ArrayTest, ValidateInsertion_Int) {
    int array[5] = {0, 1, 2, 3, 4};

    ValidateInsert<int>(array, 5);
    ValidateInsertWithIndex<int>(array, 5, -1, 0);
    ValidateInsertWithIndex<int>(array, 5, -1, 2);
    ValidateInsertWithIndex<int>(array, 5, -1, 5);
}

TEST_F(ArrayTest, ValidateInsertion_String) {
    std::string array[5] = {"a", "b", "c", "d", "e"};

    ValidateInsert<std::string>(array, 5);
    ValidateInsertWithIndex<std::string>(array, 5, "x", 0);
    ValidateInsertWithIndex<std::string>(array, 5, "x", 2);
    ValidateInsertWithIndex<std::string>(array, 5, "x", 5);
}

TEST_F(ArrayTest, ValidateRemove_Int) {
    int array[5] = {0, 1, 2, 3, 4};

    ValidateRemove(array, 5, 0);
    ValidateRemove(array, 5, 2);
    ValidateRemove(array, 5, 4);
}

TEST_F(ArrayTest, ValidateRemove_String) {
    std::string array[5] = {"a", "b", "c", "d", "e"};

    ValidateRemove(array, 5, 0);
    ValidateRemove(array, 5, 2);
    ValidateRemove(array, 5, 4);
}

TEST_F(ArrayTest, ValidateCopy) {
    int array1[5] = {0, 1, 2, 3, 4};
    ValidateCopy(array1, 5);

    std::string array2[5] = {"a", "b", "c", "d", "e"};
    ValidateCopy(array2, 5);
}

TEST_F(ArrayTest, ValidateMove) {
    int array1[5] = {0, 1, 2, 3, 4};
    ValidateMove(array1, 5);

    std::string array2[5] = {"a", "b", "c", "d", "e"};
    ValidateMove(array2, 5);
}

TEST_F(ArrayTest, ValidateIterator_Int) {
    int array[5] = {0, 1, 2, 3, 4};
    ValidateIterator(array, 5);
    ValidateReverseIterator(array, 5);
}

TEST_F(ArrayTest, ValidateIterator_String) {
    std::string array[5] = {"a", "b", "c", "d", "e"};
    ValidateIterator(array, 5);
    ValidateReverseIterator(array, 5);
}