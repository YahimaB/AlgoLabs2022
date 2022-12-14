//
// Created by Yaroslav on 12/12/2022.
//

#ifndef LAB3_QUICKSORT_SORT_H
#define LAB3_QUICKSORT_SORT_H

static int minInsertion = 12;

template<typename T, typename Compare>
void sort(T *first, T *last, Compare comp) {
    while (first < last) {
        if (last - first <= minInsertion) {
            insertionSort(first, last, comp);
            return;
        }

        T *pi = partition(first, last, comp);

        if (pi - first < last - pi) {
            sort(first, pi, comp);
            first = pi + 1;
        } else {
            sort(pi + 1, last, comp);
            last = pi;
        }
    }

}

template<typename T, typename Compare>
T *partition(T *first, T *last, Compare comp) {
    T pivot = getMedian(first, last, comp);
    T *l = first;
    T *r = last;

    while (true) {
        while (comp(*l, pivot)) {
            l++;
        }
        while (comp(pivot, *r)) {
            r--;
        }
        if (l >= r)
            break;
        std::swap(*l, *r);
        l++;
        r--;
    }

    return r;
}

template<typename T, typename Compare>
T getMedian(T *l, T *r, Compare comp) {
    T *median = l + (r - l) / 2;

    if (comp(*r, *l)) {
        std::swap(*r, *l);
    }
    if (comp(*median, *l)) {
        std::swap(*median, *l);
    }
    if (comp(*r, *median)) {
        std::swap(*r, *median);
    }
    return *median;
}

template<typename T, typename Compare>
void insertionSort(T *first, T *last, Compare comp) {
    for (T *i = first; i <= last; i++) {
        T *j = i - 1;
        T key = std::move(*i);
        while (j >= first && comp(key, *j)) {
            *(j + 1) = std::move(*j);
            j--;
        }
        *(j + 1) = std::move(key);
    }
}

template<typename T>
void printArray(T *first, T *last) {
    while (first <= last) {
        std::cout << *first << " ";
        first++;
    }
    std::cout << "\n";
}

#endif //LAB3_QUICKSORT_SORT_H
