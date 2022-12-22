//
// Created by Yaroslav on 12/22/2022.
//

#ifndef LAB4_MEMORYMANAGER_MEMORYMANAGER_H
#define LAB4_MEMORYMANAGER_MEMORYMANAGER_H


#include "FixedSizeAllocator.h"
#include "CoalesceAllocator.h"
#include "SystemAllocator.h"

class MemoryManager {
private:
    int const kSystemThreshold = 10 * 1024 * 1024;

    FixedSizeAllocator FSA_16_, FSA_32_, FSA_64_, FSA_128_, FSA_256_, FSA_512_;
    CoalesceAllocator CA_;
    SystemAllocator SA_;

    static size_t AlignSize(size_t size);

public:
    MemoryManager();

    ~MemoryManager();

    void Init();

    void *Alloc(size_t size);

    void Free(void *p);

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


#endif //LAB4_MEMORYMANAGER_MEMORYMANAGER_H
