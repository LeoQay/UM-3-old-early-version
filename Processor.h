#include "parser_um_3.h"
using namespace std;

class Processor
{
private:
    Memory mem_obj;
    Parser_UM_3 pars_obj;

    int RA;              /* счетчик адреса, содержит адрес следующей команды */
    bool Err;            /* регистр ошибок, true - произошла ошибка */
    string RK;           /* текущая команда  */
    int omega;           /* 0 - результат равен 0, 1 - меньше 0, 2 - больше 0  */

    void omega_res(int res);

    void omega_res(double res);

    void add_int(int op1, int op2, int op3);

    void sub_int(int op1, int op2, int op3);

    void send(int op1, int op3);

    void just_if(int op1, int op2, int op3);

    bool tact();

public:
    Processor();

    void input_punch_card(ifstream& fin);

    void output_memory(ofstream& fout);

    void clear_memory();

    void main_process();
};