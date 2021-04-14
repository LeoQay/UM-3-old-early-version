#include "Processor.h"

void Processor::omega_res(int res)
{
    if (res == 0) omega = 0;
    else
        if (res < 0) omega = 1;
        else
            omega = 2;
}

void Processor::omega_res(float res)
{
    if (res == 0) omega = 0;
    else
        if (res < 0) omega = 1;
        else
            omega = 2;
}

Processor::Processor()
{
    RA = 1;
    Err = false;
    omega = 0;
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
        mem_obj.push(op1++, Parser::itos(value));
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
    int res = Parser::stoi(R1) + Parser::stoi(R2);
    omega_res(res);
    S = Parser::itos(res);
    mem_obj.push(op1, S);
}

void Processor::subInt(int op1, int op2, int op3)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);
    int res = Parser::stoi(R1) - Parser::stoi(R2);
    omega_res(res);
    S = Parser::itos(res);
    mem_obj.push(op1, S);
}

void Processor::mulInt(int op1, int op2, int op3)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);
    int res = Parser::stoi(R1) * Parser::stoi(R2);
    omega_res(res);
    S = Parser::itos(res);
    mem_obj.push(op1, S);
}

void Processor::divInt(int op1, int op2, int op3)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);
    int res = Parser::stoi(R1) / Parser::stoi(R2);
    omega_res(res);
    S = Parser::itos(res);
    mem_obj.push(op1, S);
}

void Processor::modInt(int op1, int op2, int op3)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);
    int res = Parser::stoi(R1) % Parser::stoi(R2);
    omega_res(res);
    S = Parser::itos(res);
    mem_obj.push(op1, S);
}

void Processor::input_float(int op1, int op2)
{
    float value;
    while (op2-- > 0)
    {
        cin >> value;
        mem_obj.push(op1++, Parser::ftos(value));
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
    float res = (float)(Parser::stold(R1) + Parser::stold(R2));
    omega_res(res);
    S = Parser::ftos(res);
    mem_obj.push(op1, S);
}

void Processor::subFloat(int op1, int op2, int op3)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);
    float res = (float)(Parser::stold(R1) - Parser::stold(R2));
    omega_res(res);
    S = Parser::ftos(res);
    mem_obj.push(op1, S);
}

void Processor::mulFloat(int op1, int op2, int op3)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);
    float res = (float)(Parser::stold(R1) * Parser::stold(R2));
    omega_res(res);
    S = Parser::ftos(res);
    mem_obj.push(op1, S);
}

void Processor::divFloat(int op1, int op2, int op3)
{
    R1 = mem_obj.get(op2);
    R2 = mem_obj.get(op3);
    float res = (float)(Parser::stold(R1) / Parser::stold(R2));
    omega_res(res);
    S = Parser::ftos(res);
    mem_obj.push(op1, S);
}

void Processor::intToFloat(int op1, int op3)
{
    mem_obj.push(op1, Parser::ftos((float)Parser::stoi(mem_obj.get(op3))));
}

void Processor::floatToInt(int op1, int op3)
{
    mem_obj.push(op1, Parser::itos((int)Parser::stold(mem_obj.get(op3))));
}

void Processor::unconditional(int op2)
{
    RA = op2;
}

void Processor::send(int op1, int op3)
{
    mem_obj.push(op1, mem_obj.get(op3));
}

void Processor::just_if(int op1, int op2, int op3)
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
            break;
    }
}

void Processor::clear_memory()
{
    mem_obj.clear();
}

bool Processor::tact()
{
    RK = mem_obj.get(RA);
    RA = (RA + 1) % 512;

    Com command;
    int op1, op2, op3;
    Parser::pars_of_cell(RK, command, op1, op2, op3);

    switch (command)
    {
        case Com::ININT:
            input_int(op1, op2);
            break;
        case Com::OUTINT:
            output_int(op1, op2);
            break;
        case Com::ADDINT:
            addInt(op1, op2, op3);
            break;
        case Com::SUBINT:
            subInt(op1, op2, op3);
            break;
        case Com::MULINT:
            mulInt(op1, op2, op3);
            break;
        case Com::DIVINT:
            divInt(op1, op2, op3);
            break;
        case Com::MOD:
            modInt(op1, op2, op3);
            break;
        case Com::INFLOAT:
            input_float(op1, op2);
            break;
        case Com::OUTFLOAT:
            output_float(op1, op2);
            break;
        case Com::ADDFLOAT:
            addFloat(op1, op2, op3);
            break;
        case Com::SUBFLOAT:
            subFloat(op1, op2, op3);
            break;
        case Com::MULFLOAT:
            mulFloat(op1, op2, op3);
            break;
        case Com::DIVFLOAT:
            divFloat(op1, op2, op3);
            break;
        case Com::ITOF:
            intToFloat(op1, op3);
            break;
        case Com::FTOI:
            floatToInt(op1, op3);
            break;
        case Com::UNCOND:
            unconditional(op2);
            break;
        case Com::SEND:
            send(op1, op3);
            break;
        case Com::IF:
            just_if(op1, op2, op3);
            break;
        case Com::END:
            return false;
        default:
            return false;
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