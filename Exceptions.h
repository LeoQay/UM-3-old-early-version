#pragma once

#include "parser.h"

#include <iostream>

class Exceptions : std::exception
{
private:
    std::string message;
public:
    int cell_number;
    explicit Exceptions (int num, std::string message = "Error!") :
    message(move(message)), cell_number(num) {};

    const char* what() noexcept
    {
        return message.c_str();
    }
};

class Bad_cell : public Exceptions
{
public:
    explicit Bad_cell (int num, std::string message = "Bad cell!") : Exceptions(num, move(message)) {};
};

class Empty : public Bad_cell
{
public:
    explicit Empty(int num, std::string message) : Bad_cell(num, move(message)) {};
};

class Out_range : public Bad_cell
{
private:
    int bad_index;
public:
    explicit Out_range(int num, int bad_index, std::string message = "Out range!") :
    Bad_cell(num, move(message)), bad_index(bad_index) {};

    const char* what()
    {
        auto s = new std::string;
        *s = Exceptions::what();
        *s += (" " + Parser::itos(bad_index, 3, 10));
        return (*s).c_str();
    }
};

class Bad_token : public Bad_cell
{
private:
    std::string token;
public:
    explicit Bad_token(int num, std::string token, std::string message = "Bad token!") :
    Bad_cell(num, move(message)), token(move(token)) {};

    const char* what()
    {
        auto s = new std::string;
        *s = Exceptions::what();
        *s += (" " + token);
        return (*s).c_str();
    }
};

class ProcessorException : public Exceptions
{
private:
    int err_com;
    int op1;
    int op2;
    int op3;
public:
    ProcessorException (int num,int err_com, int op1, int op2, int op3, std::string message = "Runtime error!") :
    Exceptions(num, move(message)), err_com(err_com), op1(op1), op2(op2), op3(op3) {};

    const char* what()
    {
        auto s = new std::string;
        *s = Exceptions::what();

        *s += (" " + Parser::itos(err_com, 2, 10));
        *s += (" " + Parser::itos(op1, 3, 10));
        *s += (" " + Parser::itos(op2, 3, 10));
        *s += (" " + Parser::itos(op3, 3, 10));
        return (*s).c_str();
    }
};

class Bad_command : public ProcessorException
{
public:
    Bad_command (int num, int err_com, int op1, int op2, int op3, std::string message = "Bad command!") :
    ProcessorException(num, err_com, op1, op2, op3, move(message)) {};
};

class MathException : public ProcessorException
{
public:
    MathException (int num, int err_com, int op1, int op2, int op3, std::string message = "Math error!") :
    ProcessorException(num, err_com, op1, op2, op3, move(message)) {};
};

class NULL_DIVIDE : public MathException
{
public:
    NULL_DIVIDE (int num, int err_com, int op1, int op2, int op3, std::string message = "Dividing by zero is bad!") :
    MathException(num, err_com, op1, op2, op3, move(message)) {};
};