#pragma once
#include "Processor.h"
#include "Exceptions.h"

#include <cmath>

void Processor::omega_res(int res)
{
    omega = res == 0 ? 0 : res < 0 ? 1 : 2;
}

void Processor::omega_res(float res)
{
    omega = res == 0 ? 0 : res < 0 ? 1 : 2;
}

Processor::Processor()
{
    RA = 1;
    saveRA = 1;
    Err = false;
    omega = 0;
    RK = "Empty";
    R1 = "Empty";
    R2 = "Empty";
    S = "Empty";
    iterations = 0;
    max_iterations = -1;
    /* если не установить максимальное число итераций,
     * то iterations никогда не равен max_iterations */
}

void Processor::input_punched_card(ifstream& fin)
{
    pars_obj.get_punched_card(fin, &mem_obj);
}

void Processor::output_memory(ofstream& fout)
{
    for (int i = 0; i < 512; i++)
    {
        if (i < 10) fout << "  ";
        else
        if (i < 100) fout << " ";

        fout << i << " " << mem_obj.get(i) << "\n";
    }
}

void Processor::input_int(int op1, int op2)
{
    int value;
    while (op2-- > 0)
    {
        cin >> value;
        mem_obj.push(op1, Parser::itos(value));
        op1 = (op1 + 1) % 512;
    }
}

void Processor::output_int(int op1, int op2)
{
    while(op2-- > 0)
        cout << Parser::stoi(mem_obj.get(op1++)) << "\n";
}

void Processor::addInt(int op1, int op2, int op3)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);
    int I1 = Parser::stoi(R1);
    int I2 = Parser::stoi(R2);
    long long res = (long long)I1 + (long long)I2;

    if (res < -2147483648 || res > 2147483647)
    {
        Err = true;
        throw MathOutRange(saveRA, (int)Command_code::ADDINT, op1, op2, op3, I1, I2);
    }

    omega_res((int)res);
    S = Parser::itos((int)res);
    mem_obj.push(op1, S);
}

void Processor::subInt(int op1, int op2, int op3)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);
    int I1 = Parser::stoi(R1);
    int I2 = Parser::stoi(R2);
    long long res = (long long)I1 - (long long)I2;

    if (res < -2147483648 || res > 2147483647)
    {
        Err = true;
        throw MathOutRange(saveRA, (int)Command_code::SUBINT, op1, op2, op3, I1, I2);
    }

    omega_res((int)res);
    S = Parser::itos((int)res);
    mem_obj.push(op1, S);
}

void Processor::mulInt(int op1, int op2, int op3)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);
    int I1 = Parser::stoi(R1);
    int I2 = Parser::stoi(R2);
    long long res = (long long)I1 * (long long)I2;

    if (res < -2147483648 || res > 2147483647)
    {
        Err = true;
        throw MathOutRange(saveRA, (int)Command_code::MULINT, op1, op2, op3, I1, I2);
    }

    omega_res((int)res);
    S = Parser::itos((int)res);
    mem_obj.push(op1, S);
}

void Processor::divInt(int op1, int op2, int op3)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);
    int I1 = Parser::stoi(R1);
    int I2 = Parser::stoi(R2);

    if (I2 == 0) throw NULL_DIVIDE(saveRA, (int)Command_code::DIVINT, op1, op2, op3);

    long long res = (long long)I1 / (long long)I2;

    if (res < -2147483648 || res > 2147483647)
    {
        Err = true;
        throw MathOutRange(saveRA, (int)Command_code::DIVINT, op1, op2, op3, I1, I2);
    }

    omega_res((int)res);
    S = Parser::itos((int)res);
    mem_obj.push(op1, S);
}

void Processor::modInt(int op1, int op2, int op3)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);
    int I1 = Parser::stoi(R1);
    int I2 = Parser::stoi(R2);

    if (I2 == 0) throw NULL_DIVIDE(saveRA, (int)Command_code::MOD, op1, op2, op3);

    long long res = (long long)I1 % (long long)I2;

    if (res < -2147483648 || res > 2147483647)
    {
        Err = true;
        throw MathOutRange(saveRA, (int)Command_code::MOD, op1, op2, op3, I1, I2);
    }

    omega_res((int)res);
    S = Parser::itos((int)res);
    mem_obj.push(op1, S);
}

void Processor::input_float(int op1, int op2)
{
    float value;
    while (op2-- > 0)
    {
        cin >> value;
        mem_obj.push(op1, Parser::ftos(value));
        op1 = (op1 + 1) % 512;
    }
}

void Processor::output_float(int op1, int op2)
{
    while(op2-- > 0)
        cout << (float)(Parser::stold(mem_obj.get(op1++))) << "\n";
}

void Processor::addFloat(int op1, int op2, int op3)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);

    long double F1 = Parser::stold(R1);
    long double F2 = Parser::stold(R2);
    long double res = F1 + F2;

    if (abs(res) > 3.402823466 * pow(10, 38) || abs(res) < 1.175494351 * pow(10, -38))
        throw MathOutRange(saveRA, (int)Command_code::ADDFLOAT, op1, op2, op3, (float)F1, (float)F2);

    omega_res((float)res);
    S = Parser::ftos((float)res);
    mem_obj.push(op1, S);
}

void Processor::subFloat(int op1, int op2, int op3)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);
    long double F1 = Parser::stold(R1);
    long double F2 = Parser::stold(R2);
    long double res = F1 - F2;

    if (abs(res) >= 3.402823466 * pow(10, 38) || abs(res) <= 1.175494351 * pow(10, -38))
        throw MathOutRange(saveRA, (int)Command_code::SUBFLOAT, op1, op2, op3, (float)F1, (float)F2);

    omega_res((float)res);
    S = Parser::ftos((float)res);
    mem_obj.push(op1, S);
}

void Processor::mulFloat(int op1, int op2, int op3)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);
    long double F1 = Parser::stold(R1);
    long double F2 = Parser::stold(R2);
    long double res = F1 * F2;

    if (abs(res) >= 3.402823466 * pow(10, 38) || abs(res) <= 1.175494351 * pow(10, -38))
        throw MathOutRange(saveRA, (int)Command_code::MULFLOAT, op1, op2, op3, (float)F1, (float)F2);

    omega_res((float)res);
    S = Parser::ftos((float)res);
    mem_obj.push(op1, S);
}

void Processor::divFloat(int op1, int op2, int op3)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);
    long double F1 = Parser::stold(R1);
    long double F2 = Parser::stold(R2);

    if (F2 == 0) throw NULL_DIVIDE(saveRA, (int)Command_code::DIVFLOAT, op1, op2, op3);

    long double res = F1 / F2;

    if (abs(res) >= 3.402823466 * pow(10, 38) || abs(res) <= 1.175494351 * pow(10, -38))
        throw MathOutRange(saveRA, (int)Command_code::DIVFLOAT, op1, op2, op3, (float)F1, (float)F2);

    omega_res((float)res);
    S = Parser::ftos((float)res);
    mem_obj.push(op1, S);
}

void Processor::intToFloat (int op1, int op3)
{
    mem_obj.push(op1, Parser::ftos((float)Parser::stoi(mem_obj.get(op3))));
}

void Processor::floatToInt (int op1, int op3)
{
    mem_obj.push(op1, Parser::itos((int)Parser::stold(mem_obj.get(op3))));
}

void Processor::unconditional (int op2) { RA = op2; }

void Processor::PR(int op2)
{
    if (omega == 0) RA = op2;
}

void Processor::PNR (int op2)
{
    if (omega != 0) RA = op2;
}

void Processor::PB (int op2)
{
    if (omega == 2) RA = op2;
}

void Processor::PM (int op2)
{
    if (omega == 1) RA = op2;
}

void Processor::PBR (int op2)
{
    if (omega != 1) RA = op2;
}

void Processor::PMR (int op2)
{
    if (omega != 2) RA = op2;
}

void Processor::just_if (int op1, int op2, int op3)
{
    switch (omega)
    {
        case 0:
            RA = op1;
            break;
        case 1:
            RA = op2;
            break;
        case 2:
            RA = op3;
    }
}

void Processor::send(int op1, int op3)
{
    mem_obj.push(op1, mem_obj.get(op3));
}

void Processor::clear_memory()
{
    mem_obj.clear();
}

bool Processor::tact()
{
    RK = mem_obj.get(RA);
    saveRA = RA;
    RA = (RA + 1) % 512;

    Command_code command;
    int op1, op2, op3;
    Parser::pars_of_cell(RK, command, op1, op2, op3);

    switch (command)
    {
        case Command_code::ININT:
            input_int(op1, op2);
            break;
        case Command_code::OUTINT:
            output_int(op1, op2);
            break;
        case Command_code::ADDINT:
            addInt(op1, op2, op3);
            break;
        case Command_code::SUBINT:
            subInt(op1, op2, op3);
            break;
        case Command_code::MULINT:
            mulInt(op1, op2, op3);
            break;
        case Command_code::DIVINT:
            divInt(op1, op2, op3);
            break;
        case Command_code::MOD:
            modInt(op1, op2, op3);
            break;
        case Command_code::INFLOAT:
            input_float(op1, op2);
            break;
        case Command_code::OUTFLOAT:
            output_float(op1, op2);
            break;
        case Command_code::ADDFLOAT:
            addFloat(op1, op2, op3);
            break;
        case Command_code::SUBFLOAT:
            subFloat(op1, op2, op3);
            break;
        case Command_code::MULFLOAT:
            mulFloat(op1, op2, op3);
            break;
        case Command_code::DIVFLOAT:
            divFloat(op1, op2, op3);
            break;
        case Command_code::ITOF:
            intToFloat(op1, op3);
            break;
        case Command_code::FTOI:
            floatToInt(op1, op3);
            break;
        case Command_code::UNCOND:
            unconditional(op2);
            break;
        case Command_code::PR:
            PR(op2);
            break;
        case Command_code::PNR:
            PNR(op2);
            break;
        case Command_code::PB:
            PB(op2);
            break;
        case Command_code::PM:
            PM(op2);
            break;
        case Command_code::PBR:
            PBR(op2);
            break;
        case Command_code::PMR:
            PMR(op2);
            break;
        case Command_code::IF:
            just_if(op1, op2, op3);
            break;
        case Command_code::SEND:
            send(op1, op3);
            break;
        case Command_code::END:
            return false;
        default:
            throw Bad_command(saveRA, (int)command, op1, op2, op3);
    }
    return true;
}

void Processor::main_process()
{
    while (!Err && iterations++ != max_iterations && tact());
}

void Processor::set_max_iterations(int num)
{
    max_iterations = num;
}

Memory* Processor::get_Memory()
{
    return &mem_obj;
}

string Processor::output_stat()
{
    string answer;
    answer += "Register statistics:\n";
    answer += "Last command: ";
    answer += Parser::itos(RA, 3, 10) + " " + RK;
    answer += "\n";
    answer += ("R1: " + R1);
    answer += "\n";
    answer += ("R2: " + R2);
    answer += "\n";
    answer += ("Combiner: " + S);
    answer += "\n";
    answer += ("Omega: ");
    answer += (char) ('0' + omega);
    answer += "\n";
    return answer;
}