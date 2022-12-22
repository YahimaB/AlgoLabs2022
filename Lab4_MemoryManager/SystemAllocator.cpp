//
// Created by Yaroslav on 12/22/2022.
//
#include "windows.h"
#include <cassert>
#include <iostream>

#include "SystemAllocator.h"

SystemAllocator::SystemAllocator() {
#ifdef _DEBUG
    initialized_ = false;
#endif
}

SystemAllocator::~SystemAllocator() {
#ifdef _DEBUG
    assert(!initialized_ && "SA::~SystemAllocator(): not destroyed");
#endif
}

void SystemAllocator::Init() {
#ifdef _DEBUG
    assert(!initialized_ && "SA::Init(): already initialized");
    initialized_ = true;
    total_blocks_ = 0;
    freed_blocks_ = 0;
#endif
}

void *SystemAllocator::Alloc(size_t size) {
#ifdef _DEBUG
    assert(initialized_ && "SA::Alloc(): not initialized");
    total_blocks_++;
#endif

    void *p = VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    auto block = Block();
    block.data = p;
    block.data_size = size;
    blocks.push_back(block);
    return p;
}

bool SystemAllocator::Free(void *p) {
#ifdef _DEBUG
    assert(initialized_ && "SA::Free(): not initialized");
#endif

    bool released = VirtualFree(p, 0, MEM_RELEASE);
    if (released) {
        std::erase_if(blocks, [p](const Block &x) { return x.data == p; });
#ifdef _DEBUG
        freed_blocks_++;
#endif
    }
}

void SystemAllocator::Destroy() {
#ifdef _DEBUG
    assert(initialized_ && "SA::Destroy(): not initialized");
    initialized_ = false;
#endif
    for (auto &block: blocks) {
        VirtualFree(block.data, 0, MEM_RELEASE);
    }
    blocks.clear();
}

#ifdef _DEBUG

void SystemAllocator::DumpStat() const {
    std::cout << "___ SystemAllocator ___" << "\n";
    std::cout << "total allocations: " << total_blocks_ << "\n";
    std::cout << "freed allocations: " << freed_blocks_ << "\n";
}

void SystemAllocator::DumpBlocks() const {
    std::cout << "___ SystemAllocator ___" << "\n";

    int blocks_total = 0;
    for (auto &block: blocks) {
        std::cout << "Index: " << blocks_total << " | Size: " << block.data_size << " | Address: " << block.data << "\n";
        blocks_total++;
    }

    std::cout << "\n";
}

#endif


