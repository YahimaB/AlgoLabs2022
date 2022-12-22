//
// Created by Yaroslav on 12/20/2022.
//
#include <windows.h>
#include <cassert>
#include <algorithm>
#include <iostream>

#include "FixedSizeAllocator.h"

FixedSizeAllocator::FixedSizeAllocator() {
#ifdef _DEBUG
    initialized_ = false;
#endif
}

FixedSizeAllocator::~FixedSizeAllocator() {
#ifdef _DEBUG
    assert(!initialized_ && "FSA::~FixedSizeAllocator(): not destroyed");
#endif
}

FixedSizeAllocator::Page *FixedSizeAllocator::CreatePage() const {
    auto page_size = block_size_ * blocks_per_page_ + sizeof(Page);
    auto page = (Page *) VirtualAlloc(nullptr, page_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    page->buffer = (byte *) page + sizeof(Page);
    page->free_list_header = -1;
    return page;
}

void FixedSizeAllocator::Init(int block_size, int blocks_per_page) {
#ifdef _DEBUG
    assert(!initialized_ && "FSA::Init(): already initialized");
    initialized_ = true;
#endif

    block_size_ = block_size;
    blocks_per_page_ = blocks_per_page;
    first_page_ = CreatePage();
}

void *FixedSizeAllocator::Alloc() {
#ifdef _DEBUG
    assert(initialized_ && "FSA::Alloc(): not initialized");
#endif

    auto current_page = first_page_;
    while (current_page != nullptr) {
        auto blocks_initialized = &current_page->blocks_initialized;

        if (current_page->free_list_header != -1) {
            auto block_offset = current_page->free_list_header * block_size_;
            void *p = (byte *) current_page->buffer + block_offset;
            current_page->free_list_header = *(int *) p;
            return p;
        } else if (*blocks_initialized < blocks_per_page_) {
            auto block_offset = *blocks_initialized * block_size_;
            (*blocks_initialized)++;
            return (byte *) current_page->buffer + block_offset;
        }

        if (current_page->next_page == nullptr) {
            current_page->next_page = CreatePage();
        }
        current_page = current_page->next_page;
    }

    return nullptr;
}

bool FixedSizeAllocator::Free(void *p) {
#ifdef _DEBUG
    assert(initialized_ && "FSA::Free(): not initialized");
#endif

    auto current_page = first_page_;
    while (current_page != nullptr) {
        auto first_ptr = (byte *) current_page->buffer;
        auto last_ptr = (byte *) current_page->buffer + blocks_per_page_ * block_size_;

        if (first_ptr <= p && p < last_ptr) {
            *(int *) p = current_page->free_list_header;
            auto block_offset = (byte *) p - first_ptr;
            current_page->free_list_header = (int) (block_offset / block_size_);
            return true;
        }
        current_page = current_page->next_page;
    }

    return false;
}

void FixedSizeAllocator::Destroy() {
#ifdef _DEBUG
    assert(initialized_ && "FSA::Destroy(): not initialized");
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

void FixedSizeAllocator::DumpStat() const {
    int pages_total = 0;
    int blocks_initialized = 0;
    int blocks_occupied = 0;

    auto current_page = first_page_;
    while (current_page != nullptr) {
        pages_total++;
        blocks_initialized += current_page->blocks_initialized;

        auto free_blocks = GetFreeBlocks(current_page);
        for (int i = 0; i < current_page->blocks_initialized; i++) {
            if (std::find(free_blocks.begin(), free_blocks.end(), i) == free_blocks.end()) {
                blocks_occupied++;
            }
        }

        current_page = current_page->next_page;
    }

    std::cout << "___ FSA " << block_size_ << " ___" << "\n";
    std::cout << "pages: " << pages_total << " | blocks: " << pages_total * blocks_per_page_ << "\n";
    std::cout << "blocks_initialized: " << blocks_initialized << " | blocks_occupied: " << blocks_occupied << "\n";
    std::cout << "\n";
}

void FixedSizeAllocator::DumpBlocks() const {
    int pages_total = 0;

    std::cout << "___ FSA " << block_size_ << " ___" << "\n";
    auto current_page = first_page_;
    while (current_page != nullptr) {
        std::cout << " __ Page " << pages_total << " ___" << "\n";
        pages_total++;

        auto free_blocks = GetFreeBlocks(current_page);
        for (int i = 0; i < current_page->blocks_initialized; i++) {
            if (std::find(free_blocks.begin(), free_blocks.end(), i) == free_blocks.end()) {
                std::cout << "Index: " << i << " | Address: "
                          << (void *) ((byte *) (current_page->buffer) + i * block_size_) << "\n";
            }
        }

        current_page = current_page->next_page;
    }

    std::cout << "\n";
}

std::vector<int> FixedSizeAllocator::GetFreeBlocks(Page *page) const {
    std::vector<int> free_blocks;
    int header = page->free_list_header;
    while (header != -1) {
        free_blocks.push_back(header);
        header = *(int *) ((byte *) page->buffer + header * block_size_);
    }

    return free_blocks;
}

#endif





