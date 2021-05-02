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
    I1 = 0, I2 = 0;
    F1 = 0, F2 = 0;
    Summator = "Empty";
    op1 = 0, op2 = 0, op3 = 0;
    iterations = 0;
    max_iterations = -1;
    /* если не установить максимальное число итераций,
     * то iterations никогда не равен max_iterations */

    maxInt = 2147483647;
    minInt = -2147483648;
    maxFloat = 3.402823466 * pow(10, 38);
    minFloat = 1.175494351 * pow(10, -38);
}

Processor::~Processor()
{
    cout << output_stat();
}

void Processor::input_punched_card(ifstream& fin)
{
    pars_obj.get_punched_card(fin, &mem_obj);
}

void Processor::output_memory(ofstream& fout)
{
    for (int i = 0; i < 512; i++)
    {
        if (i < 10)
            fout << "  ";
        else if (i < 100)
            fout << " ";

        fout << i << " " << mem_obj.get(i) << "\n";
    }
}

void Processor::inInt()
{
    int value;
    while (op2-- > 0)
    {
        bool ok = false;

        cout << "Input int to " << op1 << " cell:";

        while (!ok)
        {
            try {
                string s = Parser::getTokenInt();
                value = Parser::stoi(s, 10);
                ok = true;
            }

            catch (Exceptions &err) {
                cout << err.what() << "\n";
                cout << "Rewrite please:";
            }
        }

        mem_obj.push(op1, Parser::itos(value));
        op1 = (op1 + 1) % 512;
    }
}

void Processor::outInt()
{
    while(op2-- > 0)
        cout << Parser::stoi(mem_obj.get(op1++)) << "\n";
}

void Processor::addInt()
{
    LoadRegisters(I1, I2);
    long long res = (long long)I1 + (long long)I2;

    OutRangeChecker(res, ADDINT);

    omega_res((int)res);
    Summator = Parser::itos((int)res);
    mem_obj.push(op1, Summator);
}

void Processor::subInt()
{
    LoadRegisters(I1, I2);
    long long res = (long long)I1 - (long long)I2;

    OutRangeChecker(res, SUBINT);

    omega_res((int)res);
    Summator = Parser::itos((int)res);
    mem_obj.push(op1, Summator);
}

void Processor::mulInt()
{
    LoadRegisters(I1, I2);
    long long res = (long long)I1 * (long long)I2;

    OutRangeChecker(res, MULINT);

    omega_res((int)res);
    Summator = Parser::itos((int)res);
    mem_obj.push(op1, Summator);
}

void Processor::divInt()
{
    LoadRegisters(I1, I2);

    if (I2 == 0) throw NULL_DIVIDE(saveRA, (int)CommandCode::DIVINT, op1, op2, op3);

    long long res = (long long)I1 / (long long)I2;

    OutRangeChecker(res, DIVINT);

    omega_res((int)res);
    Summator = Parser::itos((int)res);
    mem_obj.push(op1, Summator);
}

void Processor::modInt()
{
    LoadRegisters(I1, I2);

    if (I2 == 0) throw NULL_DIVIDE(saveRA, (int)CommandCode::MOD, op1, op2, op3);

    long long res = (long long)I1 % (long long)I2;

    OutRangeChecker(res, MOD);

    omega_res((int)res);
    Summator = Parser::itos((int)res);
    mem_obj.push(op1, Summator);
}

void Processor::inFloat()
{
    float value;

    while (op2-- > 0)
    {
        bool ok = false;

        cout << "Input float to " << op1 << " cell:";

        while (!ok)
        {
            try {
                value = Parser::getTokenFloat();
                ok = true;
            }

            catch (Exceptions &err) {
                cout << err.what() << "\n";
                cout << "Rewrite please:";
            }
        }

        mem_obj.push(op1, Parser::ftos(value));
        op1 = (op1 + 1) % 512;
    }
}

void Processor::outFloat()
{
    while(op2-- > 0)
        cout << (float)(Parser::stold(mem_obj.get(op1++))) << "\n";
}

void Processor::addFloat()
{
    LoadRegisters(F1, F2);
    long double res = F1 + F2;

    OutRangeChecker(res, ADDFLOAT);

    omega_res((float)res);
    Summator = Parser::ftos((float)res);
    mem_obj.push(op1, Summator);
}

void Processor::subFloat()
{
    LoadRegisters(F1, F2);
    long double res = F1 - F2;

    OutRangeChecker(res, SUBFLOAT);

    omega_res((float)res);
    Summator = Parser::ftos((float)res);
    mem_obj.push(op1, Summator);
}

void Processor::mulFloat()
{
    LoadRegisters(F1, F2);
    long double res = F1 * F2;

    OutRangeChecker(res, MULFLOAT);

    omega_res((float)res);
    Summator = Parser::ftos((float)res);
    mem_obj.push(op1, Summator);
}

void Processor::divFloat()
{
    LoadRegisters(F1, F2);

    if (F2 == 0) throw NULL_DIVIDE(saveRA, (int)CommandCode::DIVFLOAT, op1, op2, op3);

    long double res = F1 / F2;

    OutRangeChecker(res, DIVFLOAT);

    omega_res((float)res);
    Summator = Parser::ftos((float)res);
    mem_obj.push(op1, Summator);
}

void Processor::intToFloat ()
{
    // int is always placed in float
    mem_obj.push(op1, Parser::ftos((float)Parser::stoi(mem_obj.get(op3))));
}

void Processor::floatToInt ()
{
    long double F = Parser::stold(mem_obj.get(op3));

    if (F < minInt || F > maxInt)
    {
        Err = true;
        throw FTOIOutRange(saveRA, (int)CommandCode::FTOI, op1, op3, F);
    }

    mem_obj.push(op1, Parser::itos((int)F));
}

void Processor::unconditional ()
{
    RA = op2;
}

void Processor::PR()
{
    if (omega == 0) RA = op2;
}

void Processor::PNR ()
{
    if (omega != 0) RA = op2;
}

void Processor::PB ()
{
    if (omega == 2) RA = op2;
}

void Processor::PM ()
{
    if (omega == 1) RA = op2;
}

void Processor::PBR ()
{
    if (omega != 1) RA = op2;
}

void Processor::PMR ()
{
    if (omega != 2) RA = op2;
}

void Processor::just_if ()
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

void Processor::send()
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

    CommandCode command;

    Parser::cellParser(RK, command, op1, op2, op3);

    switch (command)
    {
        case CommandCode::ININT:
            inInt();
            break;
        case CommandCode::OUTINT:
            outInt();
            break;
        case CommandCode::ADDINT:
            addInt();
            break;
        case CommandCode::SUBINT:
            subInt();
            break;
        case CommandCode::MULINT:
            mulInt();
            break;
        case CommandCode::DIVINT:
            divInt();
            break;
        case CommandCode::MOD:
            modInt();
            break;
        case CommandCode::INFLOAT:
            inFloat();
            break;
        case CommandCode::OUTFLOAT:
            outFloat();
            break;
        case CommandCode::ADDFLOAT:
            addFloat();
            break;
        case CommandCode::SUBFLOAT:
            subFloat();
            break;
        case CommandCode::MULFLOAT:
            mulFloat();
            break;
        case CommandCode::DIVFLOAT:
            divFloat();
            break;
        case CommandCode::ITOF:
            intToFloat();
            break;
        case CommandCode::FTOI:
            floatToInt();
            break;
        case CommandCode::UNCOND:
            unconditional();
            break;
        case CommandCode::PR:
            PR();
            break;
        case CommandCode::PNR:
            PNR();
            break;
        case CommandCode::PB:
            PB();
            break;
        case CommandCode::PM:
            PM();
            break;
        case CommandCode::PBR:
            PBR();
            break;
        case CommandCode::PMR:
            PMR();
            break;
        case CommandCode::IF:
            just_if();
            break;
        case CommandCode::SEND:
            send();
            break;
        case CommandCode::END:
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
    answer += "\n-----------------------------------------------------\n";
    answer += "Register statistics:\n";
    answer += "Last command: ";
    answer += Parser::itos(saveRA, 3, 10) + " " + RK;
    answer += "\n";
    answer += ("R1      : " + R1);
    answer += "\n";
    answer += ("R2      : " + R2);
    answer += "\n";
    answer += ("Summator: " + Summator);
    answer += "\n";
    answer += ("Omega: ");
    answer += (char) ('0' + omega);
    answer += "\n";
    return answer;
}

void Processor::LoadRegisters(int &REG1, int &REG2)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);
    REG1 = Parser::stoi(R1);
    REG2 = Parser::stoi(R2);
}

void Processor::LoadRegisters(long double &REG1, long double &REG2)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);
    REG1 = Parser::stold(R1);
    REG2 = Parser::stold(R2);
}

void Processor::OutRangeChecker(long long res, CommandCode command)
{
    if (res < minInt || res > maxInt)
    {
        Err = true;
        throw MathOutRange(saveRA, (int)command, op1, op2, op3, I1, I2);
    }
}

void Processor::OutRangeChecker(long double res, CommandCode command)
{
    if (res != 0 && (abs(res) > maxFloat || abs(res) < minFloat))
    {
        Err = true;
        throw MathOutRange(saveRA, (int)command, op1, op2, op3, (float)F1, (float)F2);
    }
}