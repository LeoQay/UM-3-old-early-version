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
    max_iterations = 10000;
}

void Processor::input_punch_card(ifstream& fin)
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
        mem_obj.push(op1++, Parser_UM_3::itos(value));
    }
}

void Processor::add_int(int op1, int op2, int op3)
{
    int res = Parser_UM_3::stoi(&mem_obj.get(op2)[0]) + Parser_UM_3::stoi(&mem_obj.get(op3)[0]);
    omega_res(res);
    mem_obj.push(op1, Parser_UM_3::itos(res));
}

void Processor::sub_int(int op1, int op2, int op3)
{
    int res = Parser_UM_3::stoi(mem_obj.get(op2))
              - Parser_UM_3::stoi(mem_obj.get(op3));
    omega_res(res);
    mem_obj.push(op1, Parser_UM_3::itos(res));
}

void Processor::mul_int(int op1, int op2, int op3)
{
    int res = Parser_UM_3::stoi(mem_obj.get(op2))
              * Parser_UM_3::stoi(mem_obj.get(op3));
    omega_res(res);
    mem_obj.push(op1, Parser_UM_3::itos(res));
}

void Processor::div_int(int op1, int op2, int op3)
{
    int res = Parser_UM_3::stoi(mem_obj.get(op2))
              / Parser_UM_3::stoi(mem_obj.get(op3));
    omega_res(res);
    mem_obj.push(op1, Parser_UM_3::itos(res));
}

void Processor::add_real(int op1, int op2, int op3)
{
    float res = (float)(Parser_UM_3::stold(mem_obj.get(op2))
                        + Parser_UM_3::stold(mem_obj.get(op3)));
    omega_res(res);
    mem_obj.push(op1, Parser_UM_3::ftos(res));
}

void Processor::sub_real(int op1, int op2, int op3)
{
    float res = (float)(Parser_UM_3::stold(mem_obj.get(op2))
                        - Parser_UM_3::stold(mem_obj.get(op3)));
    omega_res(res);
    mem_obj.push(op1, Parser_UM_3::ftos(res));
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
    Parser_UM_3::pars_of_cell(RK, command, op1, op2, op3);

    switch (command)
    {
        case Com::ININT:
            input_int(op1, op2);
            break;
        case Com::ADDINT:
            add_int(op1, op2, op3);
            break;
        case Com::SUBINT:
            sub_int(op1, op2, op3);
            break;
        case Com::MULINT:
            mul_int(op1, op2, op3);
            break;
        case Com::DIVINT:
            div_int(op1, op2, op3);
            break;
        case Com::ADDREAL:
            add_real(op1, op2, op3);
            break;
        case Com::SUBREAL:
            sub_real(op1, op2, op3);
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
    while (iterations++ < max_iterations && tact());
}

void Processor::set_max_iterations(int num)
{
    max_iterations = num;
}

Memory* Processor::get_Memory()
{
    return &mem_obj;
}