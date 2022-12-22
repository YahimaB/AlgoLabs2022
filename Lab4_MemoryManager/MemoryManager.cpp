//
// Created by Yaroslav on 12/22/2022.
//
#include <cassert>
#include <stdexcept>

#include "MemoryManager.h"

MemoryManager::MemoryManager() {
#ifdef _DEBUG
    initialized_ = false;
#endif
}

MemoryManager::~MemoryManager() {
#ifdef _DEBUG
    assert(!initialized_ && "MM::~MemoryManager(): not destroyed");
#endif
}

void MemoryManager::Init() {
#ifdef _DEBUG
    assert(!initialized_ && "MM::Init(): already initialized");
    initialized_ = true;
#endif
    // вызываем инициализацию всех доступных FSA'ов и CA
    FSA_16_.Init(16);
    FSA_32_.Init(32);
    FSA_64_.Init(64);
    FSA_128_.Init(128);
    FSA_256_.Init(256);
    FSA_512_.Init(512);
    CA_.Init(kSystemThreshold); //10 MB
    SA_.Init();
}

void *MemoryManager::Alloc(size_t size) {
#ifdef _DEBUG
    assert(initialized_ && "MM::Alloc(): not initialized");
#endif
    size = AlignSize(size);
    if (size <= 16) return FSA_16_.Alloc();
    if (size <= 32) return FSA_32_.Alloc();
    if (size <= 64) return FSA_64_.Alloc();
    if (size <= 128) return FSA_128_.Alloc();
    if (size <= 256) return FSA_256_.Alloc();
    if (size <= 512) return FSA_512_.Alloc();
    if (size <= kSystemThreshold) return CA_.Alloc(size);
    return SA_.Alloc(size);
}

void MemoryManager::Free(void *p) {
#ifdef _DEBUG
    assert(initialized_ && "MM::Free(): not initialized");
#endif
    if (FSA_16_.Free(p)) return;
    if (FSA_32_.Free(p)) return;
    if (FSA_64_.Free(p)) return;
    if (FSA_128_.Free(p)) return;
    if (FSA_256_.Free(p)) return;
    if (FSA_512_.Free(p)) return;
    if (CA_.Free(p))  return;
    if (SA_.Free(p)) return;

    throw std::invalid_argument("trying to free pointer provided not by this manager");
}

void MemoryManager::Destroy() {
#ifdef _DEBUG
    assert(initialized_ && "MM::Destroy(): not initialized");
    initialized_ = false;
#endif
    FSA_16_.Destroy();
    FSA_32_.Destroy();
    FSA_64_.Destroy();
    FSA_128_.Destroy();
    FSA_256_.Destroy();
    FSA_512_.Destroy();
    CA_.Destroy();
    SA_.Destroy();
}

size_t MemoryManager::AlignSize(size_t size) {
    size_t alignment = 8;
    return (size + alignment - 1) / alignment * alignment;
}

#ifdef _DEBUG

void MemoryManager::DumpStat() const {
    FSA_16_.DumpStat();
    FSA_32_.DumpStat();
    FSA_64_.DumpStat();
    FSA_128_.DumpStat();
    FSA_256_.DumpStat();
    FSA_512_.DumpStat();
    CA_.DumpStat();
    SA_.DumpStat();
}

void MemoryManager::DumpBlocks() const {
    FSA_16_.DumpBlocks();
    FSA_32_.DumpBlocks();
    FSA_64_.DumpBlocks();
    FSA_128_.DumpBlocks();
    FSA_256_.DumpBlocks();
    FSA_512_.DumpBlocks();
    CA_.DumpBlocks();
    SA_.DumpBlocks();
}

#endif





