#include "parser.h"
#include <cmath>
using namespace std;

int Parser::stoi (std::string stroka, int origin_system)
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

std::string Parser::itos (int value, int length, int new_system)
{
    auto val = (unsigned int)value;
    string answer;

    for (int i = 0; i < length; i++)
    {
        answer.insert(0, 1, (char)('0' + val % new_system));
        val /= new_system;
    }

    return answer;
}

int Parser::command_check (string command)
{
    if (isdigit(command[0]))
        return stoi(command, 10);

    return m[command];
}

string Parser::get_token (string& s)
{
    string answer;

    int iter = 0;
    while (s[iter] == ' ') iter++;
    s.erase(0, iter);

    iter = 0;
    while (iter < s.size() && s[iter] != ' ') answer += s[iter++];
    s.erase(0, iter);

    return answer;
}

void Parser::get_punched_card (ifstream &fin, Memory* mem_obj)
{
    int position;
    while (!fin.eof())
    {
        string s, result, buffer;
        getline(fin, s);

        //   номер ячейки
        position = stoi(get_token(s), 10);

        //   команда
        result += itos(command_check(get_token(s)), 5);

        // op1, op2, op3
        for (int i = 0; i < 3; i++)
            result += itos(stoi(get_token(s), 10), 9);

        (*mem_obj).push(position, result);
    }
}

void Parser::pars_of_cell (string s, Com& command, int& op1, int& op2, int& op3)
{
    command = (Com) stoi(s.substr(0, 5));
    op1 = stoi(s.substr(5, 9));
    op2 = stoi(s.substr(14, 9));
    op3 = stoi(s.substr(23, 9));
}

long double Parser::stold (string s)
{
    if (s.substr(1, 31) == "0000000000000000000000000000000")  // 31 zero
        return 0;

    int sign = 1 - 2 * (s[0] - '0');     // при (int)s[0] = 48 sign = 1, а при 49 sign = -1
    int E = stoi(s.substr(1, 8), 2);
    int sub_Mantis = stoi('1' + s.substr(9, 23), 2);

    return sign * ((long double)sub_Mantis * pow(2, E - 127 - 23));
}

string Parser::ftos(float number) // вещественное число 1.Mantis * 2 ^ (E - 127)
{
    if (number == 0) return "00000000000000000000000000000000"; // 32 zero

    char sign = number > 0 ? '0' : '1';
    float positive = abs(number);
    int degree = 0;

    // приведение к виду 1.Mantis * 2^degree
    while (positive / 2 > 1)
    {
        positive /= 2;
        degree++;
    }
    while (positive < 1)
    {
        positive *= 2;
        degree--;
    }

    int E = 127 + degree;

    positive -= 1;  // приведение к виду 0.Mantis
    int Mantis = (int)(positive * pow(2, 23)); // приведение к виду Mantis

    string answer = sign + itos(E, 8) + itos(Mantis, 23);

    return answer;
}