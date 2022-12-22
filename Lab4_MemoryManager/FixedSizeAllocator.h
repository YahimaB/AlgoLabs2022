//
// Created by Yaroslav on 12/20/2022.
//

#ifndef LAB4_MEMORYMANAGER_FIXEDSIZEALLOCATOR_H
#define LAB4_MEMORYMANAGER_FIXEDSIZEALLOCATOR_H

#include <vector>

class FixedSizeAllocator {
private:
    struct Page {
        void *buffer = nullptr;
        int blocks_initialized = 0;
        int free_list_header = -1;
        Page *next_page = nullptr;
    };

    int block_size_ = 16;
    int blocks_per_page_ = 10;
    Page *first_page_ = nullptr;

    [[nodiscard]] Page *CreatePage() const;

public:
    FixedSizeAllocator();

    ~FixedSizeAllocator();

    void Init(int block_size);

    void Init(int block_size, int blocks_per_page);

    void *Alloc();

    bool Free(void *p);

    void Destroy();

#ifdef _DEBUG
private:
    bool initialized_;

    std::vector<int> GetFreeBlocks(Page *page) const;

public:
    void DumpStat() const;

    void DumpBlocks() const;

#else
    void DumpStat() const {};

    void DumpBlocks() const {};
#endif
};


#endif //LAB4_MEMORYMANAGER_FIXEDSIZEALLOCATOR_H
