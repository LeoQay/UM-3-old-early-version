#include "memory_cl.h"
#include <iostream>
#include <fstream>
#include <map>

enum Command_code {
    ININT = 16,
    OUTINT = 17,
    ADDINT = 11,
    SUBINT = 12,
    MULINT = 13,
    DIVINT = 14,
    MOD = 15,
    INFLOAT = 6,
    OUTFLOAT = 7,
    ADDFLOAT = 1,
    SUBFLOAT = 2,
    MULFLOAT = 3,
    DIVFLOAT = 4,
    ITOF = 30,
    FTOI = 10,
    UNCOND = 9,
    SEND = 0,
    IF = 19,
    END = 31,
    FLOAT = 40
};

typedef Command_code Com;

class Parser
{
private:
    std::map <std::string, Command_code> m = {
            {"ININT",    ININT},
            {"OUTINT", OUTINT},
            {"ADDINT",   ADDINT},
            {"SUBINT",   SUBINT},
            {"MULINT",   MULINT},
            {"DIVINT",   DIVINT},
            {"MOD", MOD},
            {"INFLOAT", INFLOAT},
            {"OUTFLOAT", OUTFLOAT},
            {"ADDFLOAT", ADDFLOAT},
            {"SUBFLOAT",  SUBFLOAT},
            {"MULFLOAT", MULFLOAT},
            {"DIVFLOAT", DIVFLOAT},
            {"ITOF",     ITOF},
            {"FTOI",     FTOI},
            {"UNCOND",   UNCOND},
            {"SEND",     SEND},
            {"IF",       IF},
            {"END",      END},
            {"FLOAT", FLOAT}
    };

    int command_check (std::string command);

    static std::string get_token (std::string& s);

public:
    static int stoi (std::string stroka, int origin_system = 2);   // string to integer

    static std::string itos (int value, int length = 32, int new_system = 2);   // integer to string

    void get_punched_card (std::ifstream &fin, Memory* mem_obj);

    static void pars_of_cell (std::string s, Com& command, int& op1, int& op2, int& op3);

    static long double stold (std::string s); // string to long double

    static std::string ftos (float number);    // float to string
};
