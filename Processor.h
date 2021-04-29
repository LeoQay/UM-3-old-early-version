#pragma once
#include "parser.h"

using namespace std;

class Processor
{
private:
    Memory mem_obj;
    Parser pars_obj;

    string R1;           /* первый регистр АЛУ */
    string R2;           /* второй регистр АЛУ */
    int I1, I2;          // регистры АЛУ для целых
    long double F1, F2;  // регистры АЛУ для вещественных
    string S;            /* регистр сумматора (хранит результат) */
    int RA;              /* счетчик адреса, содержит адрес следующей команды  */
    bool Err;            /* регистр ошибок, true - произошла ошибка  */
    string RK;           /* текущая команда  */
    int op1, op2, op3;   // current operands
    int omega;           /* 0 - результат равен 0, 1 - меньше 0, 2 - больше 0  */
    int iterations;
    int max_iterations;  /* не канонично, по умолчанию игнорируется */
    int saveRA;
    long long maxInt;
    long long minInt;
    long double maxFloat;
    long double minFloat;

    void omega_res(int res);

    void omega_res(float res);

    void inInt();

    void outInt();

    void addInt();

    void subInt();

    void mulInt();

    void divInt();

    void modInt();

    void input_float();

    void output_float();

    void addFloat();

    void subFloat();

    void mulFloat();

    void divFloat();

    void intToFloat();

    void floatToInt();

    void unconditional();

    void PR ();

    void PNR ();

    void PB ();

    void PM ();

    void PBR ();

    void PMR ();

    void just_if();

    void send();

    bool tact();

    void LoadRegisters (int& REG1, int& REG2);

    void LoadRegisters (long double& REG1, long double& REG2);

    void OutRangeChecker (long long res, Command_code command);

    void OutRangeChecker (long double res, Command_code command);

public:
    Processor();

    ~Processor();

    Memory* get_Memory();

    void input_punched_card(ifstream& fin);

    void output_memory(ofstream& fout);

    void clear_memory();

    void set_max_iterations(int num);

    void main_process();

    string output_stat();
};