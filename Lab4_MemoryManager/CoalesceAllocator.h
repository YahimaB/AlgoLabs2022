//
// Created by Yaroslav on 12/20/2022.
//

#ifndef LAB4_MEMORYMANAGER_COALESCEALLOCATOR_H
#define LAB4_MEMORYMANAGER_COALESCEALLOCATOR_H


class CoalesceAllocator {
private:
    struct Block {
        void *data = nullptr;
        Block *left_neighbour = nullptr;
        int prev_free_block_pos = -1;
        int data_size = 0;
        bool used = false;
    };

    struct Page {
        void *buffer = nullptr;
        int free_list_header = -1;
        Page *next_page = nullptr;
    };

    int page_size_ = 10;
    Page *first_page_ = nullptr;
    int nInitializedBlocks = 0;

    const int free_block_size = sizeof(Block) + sizeof(int);

    [[nodiscard]] Page *CreatePage() const;

    void CoalesceBlocks(Page *page, Block *left_block, Block *right_block);

public:
    CoalesceAllocator();

    ~CoalesceAllocator();

    void Init(int page_size);

    void *Alloc(size_t size);

    bool Free(void *p);

    void Destroy();

#ifdef _DEBUG
private:
    bool initialized_;

public:
    void DumpStat() const;

    void DumpBlocks() const;

#else
    void DumpStat() const {};

    void DumpBlocks() const {};
#endif
};


#endif //LAB4_MEMORYMANAGER_COALESCEALLOCATOR_H
