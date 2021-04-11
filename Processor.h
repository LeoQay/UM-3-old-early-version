#include "parser_um_3.h"
using namespace std;

class Processor
{
private:
    Memory mem_obj;
    Parser_UM_3 pars_obj;

    int RA;              /* счетчик адреса, содержит адрес следующей команды  */
    bool Err;            /* регистр ошибок, true - произошла ошибка  */
    string RK;           /* текущая команда  */
    int omega;           /* 0 - результат равен 0, 1 - меньше 0, 2 - больше 0  */
    int iterations;
    int max_iterations;  /* не канонично, но пока неизвестно что делает ум3 при зацикливании  */

    void omega_res(int res);

    void omega_res(float res);

    void input_int(int op1, int op2);

    void add_int(int op1, int op2, int op3);

    void sub_int(int op1, int op2, int op3);

    void mul_int(int op1, int op2, int op3);

    void div_int(int op1, int op2, int op3);

    void add_real(int op1, int op2, int op3);

    void sub_real(int op1, int op2, int op3);

    void unconditional(int op2);

    void send(int op1, int op3);

    void just_if(int op1, int op2, int op3);

    bool tact();

public:
    Processor();

    Memory* get_Memory();

    void input_punch_card(ifstream& fin);

    void output_memory(ofstream& fout);

    void clear_memory();

    void set_max_iterations(int num);

    void main_process();
};
