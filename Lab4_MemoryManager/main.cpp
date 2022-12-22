#include <iostream>
#include "FixedSizeAllocator.h"
#include "CoalesceAllocator.h"
#include "SystemAllocator.h"
#include "MemoryManager.h"

void CheckFSA() {
    auto FSA = new FixedSizeAllocator();
    FSA->Init(16, 4);
    FSA->DumpBlocks();

    int* pi = (int *) FSA->Alloc();
    FSA->DumpBlocks();
    int* pi2 = (int *) FSA->Alloc();
    int* pi3 = (int *) FSA->Alloc();
    FSA->DumpBlocks();

    FSA->Free(pi2);
    FSA->DumpBlocks();
    pi2 = (int *) FSA->Alloc();
    FSA->DumpBlocks();

    int* pi4 = (int *) FSA->Alloc();
    FSA->DumpBlocks();

    int* pi5 = (int *) FSA->Alloc();
    FSA->DumpBlocks();


    FSA->Free(pi2);
    FSA->DumpBlocks();

    pi2 = (int *) FSA->Alloc();
    FSA->DumpBlocks();

    FSA->Destroy();
    std::cout << "next destroy:" << "\n";
    FSA->Destroy();
//    FSA->DumpBlocks();
}

void CheckCA() {
    auto CA = new CoalesceAllocator();
    CA->Init(1024); //1 KB
    CA->DumpStat();

    auto p1 = CA->Alloc(800);
    std::cout << "temp";
    CA->DumpStat();

    auto p2 = CA->Alloc(500);
    CA->DumpStat();

    auto p3 = CA->Alloc(156); //156 - block divided, 157 - block saved, taken space = 192
    CA->DumpStat();

    CA->Free(p3);
    CA->DumpStat();

    CA->Free(p1);
    CA->DumpStat();

    CA->Free(p2);
    CA->DumpStat();

    p1 = CA->Alloc(600);
    p2 = CA->Alloc(500);
    p3 = CA->Alloc(20);
    auto p4 = CA->Alloc(20);
    auto p5 = CA->Alloc(20);
    auto p6 = CA->Alloc(20);
    CA->DumpStat();
    CA->DumpBlocks();

    CA->Free(p1);
    CA->Free(p2);
    CA->Free(p4);
    CA->Free(p3);
    CA->DumpStat();
    CA->DumpBlocks();

    CA->Free(p5);
    CA->Free(p6);
    CA->DumpStat();

    CA->Destroy();
    CA->DumpStat();
}

void CheckSA() {
    auto SA = new SystemAllocator();
    SA->Init();

    auto p1 = SA->Alloc(50);
    auto p2 = SA->Alloc(130);
    SA->DumpStat();
    SA->DumpBlocks();

    SA->Free(p1);
    SA->DumpStat();
    SA->DumpBlocks();
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    MemoryManager MM;
    MM.Init();

    int *pi = (int *)MM.Alloc(sizeof(int));
    double *pd = (double *)MM.Alloc(sizeof(double));
    int *pa = (int *)MM.Alloc(10 * sizeof(int));
    auto pm = MM.Alloc(1024);
    MM.DumpStat();
    MM.DumpBlocks();
    MM.Free(pa);
    MM.Free(pd);
    MM.Free(pi);
    MM.Free(pm);
    MM.Destroy();

    return 0;
}
