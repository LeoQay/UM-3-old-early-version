#include "Memory.h"
#include <iostream>
#include <fstream>
#include <map>

enum Command_code {
    ADDINT = 11,
    SUBINT = 12,
    SEND = 0,
    IF = 19,
    END = 31
};

typedef Command_code Com;

class Parser_UM_3
{
private:
    std::map <std::string, Command_code> m = {
            {"ADDINT", ADDINT},
            {"SUBINT", SUBINT},
            {"SEND",   SEND},
            {"IF", IF},
            {"END", END}
    };

    int com_check(std::string command);

    static std::string get_token(std::string& s);

public:
    static int my_stoi (std::string stroka, int origin_system = 2);

    static std::string my_itos (int value, int length = 32, int new_system = 2);

    void get_punch_card(std::ifstream&, Memory* mem_obj);

    static void pars_of_cell(std::string& s, Com& command, int& op1, int& op2, int& op3);
};