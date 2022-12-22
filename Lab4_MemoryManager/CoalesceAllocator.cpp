//
// Created by Yaroslav on 12/20/2022.
//
#include <windows.h>
#include <iostream>
#include <cassert>
#include <vector>

#include "CoalesceAllocator.h"

CoalesceAllocator::CoalesceAllocator() {
#ifdef _DEBUG
    initialized_ = false;
#endif
}

CoalesceAllocator::~CoalesceAllocator() {
#ifdef _DEBUG
    assert(!initialized_ && "CA::~CoalesceAllocator(): not destroyed");
#endif
}

CoalesceAllocator::Page *CoalesceAllocator::CreatePage() const {
    auto init_block_size = sizeof(Block) + page_size_;

    auto page_size = sizeof(Page) + init_block_size;
    auto page = (Page *) VirtualAlloc(nullptr, page_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    page->buffer = (byte *) page + sizeof(Page);

    auto init_block = (Block *) page->buffer;
    init_block->data_size = page_size_;

    init_block->data = (byte *) init_block + sizeof(Block);
    *(int *) init_block->data = -1;

    init_block->left_neighbour = nullptr;
    init_block->prev_free_block_pos = -1;

    page->free_list_header = 0;
    return page;
}

void CoalesceAllocator::Init(int page_size) {
#ifdef _DEBUG
    assert(!initialized_ && "CA::Init(): already initialized");
    initialized_ = true;
#endif
    page_size_ = page_size;
    first_page_ = CreatePage();
}

void *CoalesceAllocator::Alloc(size_t size) {
#ifdef _DEBUG
    assert(initialized_ && "CA::Alloc(): not initialized");
#endif

    auto current_page = first_page_;
    while (current_page != nullptr) {
        Block *prev_free_block = nullptr;

        auto current_free_block_pos = current_page->free_list_header;
        while (current_free_block_pos != -1) {
            auto current_free_block = (Block *) ((byte *) current_page->buffer + current_free_block_pos);
            int next_free_block_pos = *(int *) current_free_block->data;
            if (current_free_block->data_size >= size) {
                if (current_free_block->data_size >= size + kFreeBlockSize) {
                    auto new_block = (Block *) ((byte *) current_free_block->data + size);
                    new_block->left_neighbour = current_free_block;
                    new_block->data = (byte *) new_block + sizeof(Block);
                    new_block->data_size = current_free_block->data_size - size - sizeof(Block);
                    new_block->used = false;
                    *(int *) new_block->data = next_free_block_pos;
                    next_free_block_pos = current_free_block_pos + size + sizeof(Block);
                    current_free_block->data_size = size;
                }

                if (prev_free_block == nullptr) {
                    current_page->free_list_header = next_free_block_pos;
                } else {
                    *(int *) prev_free_block->data = next_free_block_pos;
                }

                current_free_block->used = true;
                return current_free_block->data;
            }
            prev_free_block = current_free_block;
            current_free_block_pos = next_free_block_pos;
        }

        if (current_page->next_page == nullptr) {
            current_page->next_page = CreatePage();
        }
        current_page = current_page->next_page;
    }

    return nullptr;
}

bool CoalesceAllocator::Free(void *p) {
#ifdef _DEBUG
    assert(initialized_ && "CA::Free(): not initialized");
#endif

    auto current_page = first_page_;
    while (current_page != nullptr) {
        auto first_ptr = (byte *) current_page->buffer;
        auto last_ptr = (byte *) current_page->buffer + page_size_;

        if (first_ptr <= p && p < last_ptr) {
            auto current_block = (Block *) ((byte *) p - sizeof(Block));
            current_block->used = false;

            auto right_block = (Block *) ((byte *) p + current_block->data_size);
            if ((byte *) right_block < last_ptr) {
                if (!right_block->used) {
                    CoalesceBlocks(current_page, current_block, right_block);

                    right_block = (Block *) ((byte *) p + current_block->data_size);
                    if ((byte *) right_block >= last_ptr) {
                        right_block = nullptr;
                    }
                }
            } else {
                right_block = nullptr;
            }

            auto left_block = current_block->left_neighbour;
            if (left_block != nullptr && !left_block->used) {
                left_block->data_size = left_block->data_size + current_block->data_size + sizeof(Block);
                if (right_block != nullptr) {
                    right_block->left_neighbour = left_block;
                }
            } else {
                *(int *) current_block->data = current_page->free_list_header;
                current_block->prev_free_block_pos = -1;
                if (right_block != nullptr) {
                    right_block->left_neighbour = current_block;
                }
                current_page->free_list_header = (byte *) current_block - (byte *) current_page->buffer;
            }

            return true;
        }

        current_page = current_page->next_page;
    }

    return false;
}

void CoalesceAllocator::CoalesceBlocks(Page *page, Block *left_block, Block *right_block) {
    //remove right block from list
    int right_prev_block_pos = right_block->prev_free_block_pos;
    int right_next_block_pos = *(int *) right_block->data;

    if (right_prev_block_pos != -1) {
        auto right_prev_block = (Block *) ((byte *) page->buffer + right_prev_block_pos);
        *(int *) right_prev_block->data = right_next_block_pos;

        if (right_next_block_pos != -1) {
            auto right_next_block = (Block *) ((byte *) page->buffer + right_prev_block_pos);
            right_next_block->prev_free_block_pos = right_prev_block_pos;
        }
    }

    left_block->data_size = left_block->data_size + right_block->data_size + sizeof(Block);
}

void CoalesceAllocator::Destroy() {
#ifdef _DEBUG
    assert(initialized_ && "CA::Destroy(): not initialized");
    initialized_ = false;
#endif

    auto current_page = first_page_;
    while (current_page != nullptr) {
        auto page_to_destroy = current_page;
        current_page = current_page->next_page;
        VirtualFree((void *) page_to_destroy, 0, MEM_RELEASE);
    }

    first_page_ = nullptr;
}

#ifdef _DEBUG

void CoalesceAllocator::DumpStat() const {
    int pages_total = 0;
    std::vector<int> free_blocks_sizes;
    std::vector<int> used_blocks_sizes;

    auto current_page = first_page_;
    while (current_page != nullptr) {
        pages_total++;

        auto current_block = (Block *) current_page->buffer;
        while ((byte *) current_block - (byte *) current_page->buffer < page_size_ + sizeof(Block)) {
            if (current_block->used) {
                used_blocks_sizes.push_back(current_block->data_size);
            } else {
                free_blocks_sizes.push_back(current_block->data_size);
            }

            current_block = (Block *) ((byte *) current_block->data + current_block->data_size);
        }

        current_page = current_page->next_page;
    }

    std::cout << "___ CA ___" << "\n";
    std::cout << "pages: " << pages_total << " | blocks: " << free_blocks_sizes.size() + used_blocks_sizes.size()
              << "\n";
    std::cout << "free_blocks_sizes: ";
    for (auto i: free_blocks_sizes) {
        std::cout << i << ", ";
    }
    std::cout << "\n";

    std::cout << "used_blocks_sizes: ";
    for (auto i: used_blocks_sizes) {
        std::cout << i << ", ";
    }
    std::cout << "\n";
    std::cout << "\n";
}

void CoalesceAllocator::DumpBlocks() const {
    std::cout << "___ CA ___" << "\n";

    int pages_total = 0;
    auto current_page = first_page_;
    while (current_page != nullptr) {
        std::cout << " __ Page " << pages_total << " ___" << "\n";
        pages_total++;

        int blocks_total = 0;
        auto current_block = (Block *) current_page->buffer;
        while ((byte *) current_block - (byte *) current_page->buffer < page_size_ + sizeof(Block)) {
            std::cout << "Index: " << blocks_total << " | Used: " << (current_block->used ? "true" : "false") << " | Size: " << sizeof(Block)
                      << "+" << current_block->data_size << " | Address: " << current_block << "\n";
            blocks_total++;
            current_block = (Block *) ((byte *) current_block->data + current_block->data_size);
        }

        current_page = current_page->next_page;
    }

    std::cout << "\n";
}

#endif



