#pragma once

#include "parser.h"

#include <iostream>
#include <sstream>

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

class ParserException : public Exceptions
{
public:
    explicit ParserException (int num, std::string message = "Bad cell!") :
                              Exceptions(num, move(message)) {};
};

class Empty : public ParserException
{
public:
    explicit Empty(int num, std::string message) :
                   ParserException(num, move(message)) {};
};

class IndexOutRange : public ParserException
{
private:
    int bad_index;
public:
    explicit IndexOutRange(int num, int bad_index, std::string message = "Out range!") :
            ParserException(num, move(message)), bad_index(bad_index) {};

    const char* what()
    {
        auto s = new std::string;
        *s = Exceptions::what();
        std::ostringstream oss;
        oss << bad_index;
        *s += (" " + oss.str());
        return (*s).c_str();
    }
};

class Bad_token : public ParserException
{
private:
    std::string token;
public:
    explicit Bad_token(int num, std::string token, std::string message = "Bad token!") :
            ParserException(num, move(message)), token(move(token)) {};

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
        *s += "\nCell:";
        std::ostringstream oss;
        oss << " " << err_com << " " << op1 << " " << op2 << " " << op3;
        (*s) += oss.str();
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
private:
    int iop2;
    int iop3;
    float fop2;
    float fop3;
public:
    MathException (int num, int err_com, int op1, int op2, int op3, std::string message = "Math error!") :
                   ProcessorException(num, err_com, op1, op2, op3, move(message)),
                   iop2(1), iop3(1), fop2(1), fop3(1) {};

    MathException (int num, int err_com, int op1, int op2, int op3,
                   int iop2, int iop3, std::string message = "Math error!") :
                   ProcessorException(num, err_com, op1, op2, op3, move(message)),
                   iop2(iop2), iop3(iop3), fop2(1), fop3(1) {};

    MathException (int num, int err_com, int op1, int op2, int op3,
                   float fop2, float fop3, std::string message = "Math error!") :
                   ProcessorException(num, err_com, op1, op2, op3, move(message)),
                   fop2(fop2), fop3(fop3), iop2(1), iop3(1) {};

    const char* what ()
    {
        auto s = new std::string;
        *s = ProcessorException::what();

        if (iop2 == 1 && iop3 == 1 && fop2 == 1 && fop3 == 1)
            return (*s).c_str();
        else
            if (iop2 == 1 && iop3 == 1)
            {
                std::ostringstream oss;
                oss << "\nfloat: op2 = " << fop2 << ", op3 = " << fop3;
                *s += oss.str();
                return (*s).c_str();
            }else
            {
                std::ostringstream oss;
                oss << "\nint: op2 = " << iop2 << ", op3 = " << iop3;
                *s += oss.str();
                return (*s).c_str();
            }
    }
};

class NULL_DIVIDE : public MathException
{
public:
    NULL_DIVIDE (int num, int err_com, int op1, int op2, int op3, std::string message = "Dividing by zero is bad!") :
    MathException(num, err_com, op1, op2, op3, move(message)) {};
};

class MathOutRange : public MathException
{
public:
    // простой ответ
    MathOutRange (int num, int err_com, int op1, int op2, int op3, std::string message = "Math out range!") :
                  MathException(num, err_com, op1, op2, op3, move(message)) {};

    // ответ для операций с целыми
    MathOutRange (int num, int err_com, int op1, int op2, int op3,
                  int iop2, int iop3, std::string message = "Math out range!") :
                  MathException(num, err_com, op1, op2, op3, iop2, iop3, move(message)) {};

    // ответ для операций с вещественными
    MathOutRange (int num, int err_com, int op1, int op2, int op3,
                  float fop2, float fop3, std::string message = "Math out range!") :
                  MathException(num, err_com, op1, op2, op3, fop2, fop3, move(message)) {};
};

class FTOIOutRange : public MathOutRange
{
private:
    long double res;
public:
    FTOIOutRange (int num, int err_com, int op1, int op3, long double res,
                  std::string message = "Float to int out range!") :
                  MathOutRange(num, err_com, op1, 0, op3, move(message)), res(res) {}

    const char* what ()
    {
        auto answer = new std::string;
        *answer = MathException::what();
        std::ostringstream oss;
        oss << "\n" << "Bad value: " << res;
        *answer += oss.str();
        return (*answer).c_str();
    }
};