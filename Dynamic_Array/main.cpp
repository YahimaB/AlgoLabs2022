#include <iostream>
#include "Array.h"

int main() {
    Array<int> a = Array<int>(0);
    for (int i = 0; i < 4; ++i) {
        a.insert(i + 1);
    }

    const Array<int> b = a;

    for (int i = 4; i < 6; ++i) {
        a.insert(i + 1);
    }
    for (auto it = a.iterator(); it.hasNext(); it.next()) {
        it.set(it.get() + 1);
    }

    std::cout << "Array a:" << std::endl;
    std::cout << "capacity = " << a.capacity() << " | size = " << a.size() << std::endl;
    for (auto it = a.iterator(); it.hasNext(); it.next()) {
        std::cout << it.get() << std::endl;
    }

    std::cout << "Array b:" << std::endl;
    std::cout << "capacity = " << b.capacity() << " | size = " << b.size() << std::endl;
    for (auto it = b.reverseIterator(); it.hasNext(); it.next()) {
        std::cout << it.get() << std::endl;
    }

    return 0;
}
