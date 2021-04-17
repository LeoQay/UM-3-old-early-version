#pragma once
#include "parser.h"
#include "Exceptions.h"

#include <cmath>
using namespace std;

int Parser::command_check (string command, int num)
{
    if (isdigit(command[0]))
    {
        int iter = 0;
        while (isdigit(command[iter]) && iter < command.length()) iter++;

        if (iter == command.length())
            return stoi(command, 10);
        else
            throw Bad_token(num, command);
    }

    if (m.find(command) == m.end()) throw Bad_token(num, command);

    return m[command];
}

string Parser::get_token (string& s, int num)
{
    string answer;

    int iter = 0;
    while (s[iter] == ' ' && iter < s.length()) iter++;

    if (iter == s.length()) throw Empty(num, "Empty");

    s.erase(0, iter);

    iter = 0;
    while (iter < s.size() && s[iter] != ' ') answer += s[iter++];
    s.erase(0, iter);

    return answer;
}

void Parser::get_punched_card (ifstream &fin, Memory* mem_obj)
{
    int number_cell = 0;
    while (!fin.eof())
    {
        number_cell++;
        int position;
        string s, result, buffer;
        getline(fin, s);

        //   номер ячейки
        try{ buffer = get_token(s, number_cell); }
        catch (Empty&) { throw Empty(number_cell, "Empty number!");}

        if (!number(buffer)) throw Bad_token(number_cell, buffer);

        position = stoi(buffer, 10);
        if (position >= 512 || position <= -1) throw Out_range(number_cell, position);

        //   команда
        try{ buffer = get_token(s, number_cell); }
        catch (Exceptions&) { throw Empty(number_cell, "Empty command!");}

        result += itos(command_check(buffer, number_cell), 5);

        // op1, op2, op3
        for (int i = 0; i < 3; i++)
        {
            string opi = "op";
            opi += char('1' + i);

            try{ buffer = get_token(s, number_cell); }
            catch (Empty&) { throw Empty(number_cell, "Empty " + opi + "!"); }

            if (!number(buffer)) throw Bad_token(number_cell, buffer);

            int token_val = stoi(buffer, 10);

            if (token_val >= 512 || token_val <= -1)
                throw Out_range(number_cell, token_val, "Out range " + opi + "!");

            result += itos(token_val, 9);
        }

        (*mem_obj).push(position, result);
    }
}

bool Parser::number(string& s)
{
    int i = 0;
    while (((i == 0 && s[0] == '-') || isdigit(s[i])) && i < s.length()) i++;

    return i == s.length();
}

void Parser::pars_of_cell (string& s, Command_code& command, int& op1, int& op2, int& op3)
{
    command = (Command_code) stoi(s.substr(0, 5));
    op1 = stoi(s.substr(5, 9));
    op2 = stoi(s.substr(14, 9));
    op3 = stoi(s.substr(23, 9));
}

int Parser::stoi (std::string stroka, int origin_system)
{
    unsigned int num = 0, ten_sys, it = 0;
    int sign = 1;

    if (stroka[0] == '-')
    {
        sign = -1;
        stroka.erase(0, 1);
    }

    while (it < stroka.size())
    {
        ten_sys = (int)stroka[it] - '0';
        num = origin_system * num + ten_sys;
        it++;
    }

    return (signed int)num * sign;
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

long double Parser::stold (string s)
{
    if (s.substr(1, 31) == "0000000000000000000000000000000")  // 31 zero
        return 0;

    int sign = 1 - 2 * (s[0] - '0');     // при (int)s[0] = 48 sign = 1, а при 49 sign = -1
    int E = stoi(s.substr(1, 8), 2);
    int sub_Mantis = stoi('1' + s.substr(9, 23), 2);

    return sign * ((long double)sub_Mantis * pow(2, E - 127 - 23));
}

string Parser::ftos(float number) /* вещественное число 1.Mantis * (2 ^ (E - 127)) */
{
    if (number == 0) return "00000000000000000000000000000000"; // 32 zero

    char sign = number > 0 ? '0' : '1';
    float positive = abs(number);
    int degree = 0;

    // приведение к виду 1.Mantis * 2^degree
    while (positive / 2 >= 1)
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