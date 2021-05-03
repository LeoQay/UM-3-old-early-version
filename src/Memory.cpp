#pragma once
#include "Memory.h"
#include "Exception.h"

#include <sstream>

Memory::Memory()
{
    for (auto & var : mem_mas)
    {
        var.val = "";
        var.empty = true;
    }
}

std::string Memory::get(int index)
{
    if (mem_mas[index].empty == true)
        throw memoryUndefined(0, index);

    return mem_mas[index].val;
}

void Memory::push(int index, std::string new_val)
{
    mem_mas[index].val = std::move(new_val);
    mem_mas[index].empty = false;
}

std::string Memory::outMemory()
{
    std::ostringstream answer;
    for (int i = 0; i < 512; i++)
        if (!mem_mas[i].empty)
        {
            if (i < 10)
                answer << "  ";
            else if (i < 100)
                answer << " ";

            answer << i << " " << mem_mas[i].val << "\n";
        }
    return answer.str();
}