#include "parser.h"
using namespace std;

class Processor
{
private:
    Memory mem_obj;
    Parser pars_obj;

    string R1;           /* первый регистр АЛУ */
    string R2;           /* второй регистр АЛУ */
    string S;            /* регистр сумматора (хранит результат) */
    int RA;              /* счетчик адреса, содержит адрес следующей команды  */
    bool Err;            /* регистр ошибок, true - произошла ошибка  */
    string RK;           /* текущая команда  */
    int omega;           /* 0 - результат равен 0, 1 - меньше 0, 2 - больше 0  */
    int iterations;
    int max_iterations;  /* не канонично, по умолчанию игнорируется */

    void omega_res(int res);

    void omega_res(float res);

    void input_int(int op1, int op2);

    void output_int(int op1, int op2);

    void addInt(int op1, int op2, int op3);

    void subInt(int op1, int op2, int op3);

    void mulInt(int op1, int op2, int op3);

    void divInt(int op1, int op2, int op3);

    void modInt(int op1, int op2, int op3);

    void input_float(int op1, int op2);

    void output_float(int op1, int op2);

    void addFloat(int op1, int op2, int op3);

    void subFloat(int op1, int op2, int op3);

    void mulFloat(int op1, int op2, int op3);

    void divFloat(int op1, int op2, int op3);

    void intToFloat(int op1, int op3);

    void floatToInt(int op1, int op3);

    void unconditional(int op2);

    void send(int op1, int op3);

    void just_if(int op1, int op2, int op3);

    bool tact();

public:
    Processor();

    Memory* get_Memory();

    void input_punched_card(ifstream& fin);

    void output_memory(ofstream& fout);

    void clear_memory();

    void set_max_iterations(int num);

    void main_process();
};