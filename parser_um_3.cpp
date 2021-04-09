#include "parser_um_3.h"
using namespace std;

int Parser_UM_3::my_stoi (std::string stroka, int origin_system)
{
    unsigned int num = 0, ten_sys, it = 0;

    while (it < stroka.size())
    {
        ten_sys = (int)stroka[it] - '0';
        num = origin_system * num + ten_sys;
        it++;
    }

    return (signed int)num;
}

std::string Parser_UM_3::my_itos (int value, int length, int new_system)
{
    auto val = (unsigned int)value;
    string answer;

    for (int i = 0; i < length; i++)
    {
        answer = (char)('0' + val % new_system) + answer;
        val /= new_system;
    }

    return answer;
}

int Parser_UM_3::com_check(string command)
{
    if (isdigit(command[0]))
        return my_stoi(command, 10);

    return m[command];
}

string Parser_UM_3::get_token(string& s)
{
    string answer;

    int iter = 0;
    while (s[iter] == ' ') iter++;
    s.erase(0, iter);

    iter = 0;
    while (s[iter] != ' ' && iter < s.size()) answer += s[iter++];
    s.erase(0, iter);

    return answer;
}

void Parser_UM_3::get_punch_card(ifstream& fin, Memory* mem_obj)
{
    int position;
    while (!fin.eof())
    {
        string s, result;
        getline(fin, s);

        //   номер ячейки
        position = my_stoi(get_token(s), 10);

        //   команда
        result += my_itos(com_check(get_token(s)), 5);

        // op1, op2, op3
        for (int i = 0; i < 3; i++)
            result += my_itos(my_stoi(get_token(s), 10), 9);

        (*mem_obj).push(position, result);
    }
}

void Parser_UM_3::pars_of_cell(string& s, Com& command, int& op1, int& op2, int& op3)
{
    command = (Com) my_stoi(s.substr(0, 5));
    op1 = my_stoi(s.substr(5, 9));
    op2 = my_stoi(s.substr(14, 9));
    op3 = my_stoi(s.substr(23, 9));
}