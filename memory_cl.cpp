#include "memory_cl.h"
#include <iostream>
#include "Exceptions.h"

Memory::Memory()
{
    for (auto & var : mem_mas)
        var = "00000000000000000000000000000000";
}

std::string Memory::get(int index)
{
    return mem_mas[index];
}

void Memory::push(int index, std::string new_val)
{
    mem_mas[index] = std::move(new_val);
}

void Memory::clear()
{
    for (auto & var : mem_mas)
        var = "00000000000000000000000000000000";
}