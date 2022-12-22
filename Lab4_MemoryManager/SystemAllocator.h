//
// Created by Yaroslav on 12/22/2022.
//

#ifndef LAB4_MEMORYMANAGER_SYSTEMALLOCATOR_H
#define LAB4_MEMORYMANAGER_SYSTEMALLOCATOR_H


#include <vector>

class SystemAllocator {
private:
    struct Block {
        void *data = nullptr;
        size_t data_size = 0;
    };

    std::vector<Block> blocks;
public:

    SystemAllocator();

    ~SystemAllocator();

    void Init();

    void *Alloc(size_t size);

    bool Free(void *p);

    void Destroy();

#ifdef _DEBUG
private:
    bool initialized_;
    int freed_blocks_;
    int total_blocks_;

public:
    void DumpStat() const;

    void DumpBlocks() const;

#else
    void DumpStat() const {};

    void DumpBlocks() const {};
#endif
};


#endif //LAB4_MEMORYMANAGER_SYSTEMALLOCATOR_H
