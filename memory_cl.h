#pragma once
#include <iostream>
#include <fstream>

class Memory
{
private:
    int mem_size = 512;
    std::string mem_mas[512];
public:
    Memory();

    std::string get(int index);

    void push(int index, std::string new_val);

    void clear();
};