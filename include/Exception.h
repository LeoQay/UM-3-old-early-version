#pragma once

#include "parser.h"

#include <iostream>
#include <sstream>

class Exception : std::exception
{
protected:
    std::string message;
public:
    int cell_number;
    explicit Exception (int num, std::string message = "Error!") :
                         message(move(message)), cell_number(num) {};

    const char* what() noexcept
    {
        return message.c_str();
    }
};

class ParserException : public Exception
{
public:
    explicit ParserException (int num, std::string message = "Bad cell!") :
                              Exception(num, move(message)) {};
};

class Empty : public ParserException
{
public:
    Empty(int num, std::string message) :
                   ParserException(num, move(message)) {};
};

class IndexOutRange : public ParserException
{
public:
    IndexOutRange(int num, int bad_index, std::string message = "Out range!") :
            ParserException(num, move(message))
    {
        std::ostringstream oss;
        oss << bad_index;
        this->message += (" " + oss.str());
    }
};

class Bad_token : public ParserException
{
public:
    explicit Bad_token(int num, std::string& token, std::string message = "Bad token!") :
            ParserException(num, move(message))
    {
        this->message += (" " + token);
    }
};

class ProcessorException : public Exception
{
public:
    ProcessorException (int num,int err_com, int op1, int op2, int op3, std::string message = "Runtime error!") :
    Exception(num, move(message))
    {
        std::ostringstream oss;
        oss << "\nCell: " << err_com << " " << op1 << " " << op2 << " " << op3;
        this->message += oss.str();
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

    MathException (int num, int err_com, int op1, int op2, int op3,
                   int iop2, int iop3, std::string message = "Math error!") :
                   ProcessorException(num, err_com, op1, op2, op3, move(message))
    {
        std::ostringstream oss;
        oss << "\nint: op2 = " << iop2 << ", op3 = " << iop3;
        message += oss.str();
    }

    MathException (int num, int err_com, int op1, int op2, int op3,
                   float fop2, float fop3, std::string message = "Math error!") :
                   ProcessorException(num, err_com, op1, op2, op3, move(message))
    {
        std::ostringstream oss;
        oss << "\nfloat: op2 = " << fop2 << ", op3 = " << fop3;
        message += oss.str();
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
public:
    FTOIOutRange (int num, int err_com, int op1, int op2, int op3, long double res,
                  std::string message = "Float to int out range!") :
                  MathOutRange(num, err_com, op1, op2, op3, move(message))
    {
        std::ostringstream oss;
        oss << "\n" << "Bad value: " << res;
        this->message += oss.str();
    }
};

class MemoryException : public Exception
{
public:
    MemoryException (int num, int address, std::string message = "Memory exception!") :
            Exception(num, move(message))
    {
        std::ostringstream oss;
        oss << "\nAddress: " << address;
        this->message += oss.str();
    }
};

class memoryUndefined : public MemoryException
{
public:
    int address;
    memoryUndefined (int num, int address, std::string message = "Accessing Undefined Memory!") :
            MemoryException(num, address, move(message)) { this->address = address; }
};