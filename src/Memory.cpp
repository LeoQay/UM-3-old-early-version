#pragma once
#include "Memory.h"
#include "Exception.h"

#include <iostream>

Memory::Memory()
{
    for (auto & var : mem_mas)
        var = "Empty";
}

std::string Memory::get(int index)
{
    if (mem_mas[index] == "Empty")
        throw memoryUndefined(0, index);

    return mem_mas[index];
}

void Memory::push(int index, std::string new_val)
{
    mem_mas[index] = std::move(new_val);
}

void Memory::clear()
{
    for (auto & var : mem_mas)
        var = "Empty";
}

void Memory::outMemory(std::ofstream &fout)
{
    for (int i = 0; i < 512; i++)
        if (mem_mas[i] != "Empty")
        {
            if (i < 10)
                fout << "  ";
            else if (i < 100)
                fout << " ";

            fout << i << " " << mem_mas[i] << "\n";
        }
}