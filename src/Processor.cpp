#pragma once
#include "Processor.h"
#include "Exception.h"

#include <cmath>

void Processor::omega_res(int res)
{
    omega = res == 0 ? 0 : res < 0 ? 1 : 2;
}

void Processor::omega_res(float res)
{
    omega = res == 0 ? 0 : res < 0 ? 1 : 2;
}

Processor::Processor(ofstream& log) : logFile(log)
{
    RA = 1;
    saveRA = 1;
    Err = false;
    omega = 0;
    RK = "Empty";
    RKcommand = END;
    R1 = "Empty";
    R2 = "Empty";
    I1 = 0, I2 = 0;
    F1 = 0, F2 = 0;
    Summator = "Empty";
    op1 = 0, op2 = 0, op3 = 0;
    iterations = 0;
    max_iterations = -1;
    /* если не установить максимальное число итераций,
     * то iterations никогда не равен max_iterations */

    maxInt = 2147483647ll;
    minInt = -2147483648ll;
    maxFloat = 3.402823466 * pow(10, 38);
    minFloat = 1.175494351 * pow(10, -38);
}

Processor::~Processor()
{
    string s;
    s = output_stat();
    logFile << s;
    cout << s;
}

void Processor::Input_PunchedCard(ifstream& fin)
{
    pars_obj.get_punched_card(fin, memory);
    logFile << "Punched card was loaded successfully\n\n";
}

string Processor::outMemory()
{
    return memory.outMemory();
}

void Processor::inInt()
{
    int value;
    while (op2-- > 0)
    {
        bool ok = false;

        logFile << "Input int to " << "<"<<op1<<">" << ":";
        cout << "Input int to " << "<"<<op1<<">" << ":";

        while (!ok)
        {
            try {
                string token = Parser::getTokenInt();
                logFile << token << "\n";
                value = Parser::stoi(token, 10);
                ok = true;
            }

            catch (Exception &err) {
                cout << err.what() << "\nRewrite please:";
            }
        }

        memory.push(op1, Parser::itos(value));
        op1 = (op1 + 1) % 512;
    }
}

void Processor::outInt()
{
    while(op2-- > 0)
    {
        int val = Parser::stoi(memory.get(op1++));
        logFile << "Int from " << "<"<<op1<<">" << ": " << val << "\n";
        cout << "Int from " << "<"<<op1<<">" << ": " << val << "\n";
    }
}

void Processor::addInt()
{
    LoadRegisters(I1, I2, "ADDINT", "+");

    long long res = (long long)I1 + (long long)I2;

    OutRangeChecker(res, ADDINT);

    omega_res((int)res);
    Summator = Parser::itos((int)res);
    memory.push(op1, Summator);
}

void Processor::subInt()
{
    if (op2 == op3)
    {
        SubSelfSelf();
    }else
    {
        LoadRegisters(I1, I2, "SUBINT", "-");

        long long res = (long long)I1 - (long long)I2;

        OutRangeChecker(res, SUBINT);

        omega_res((int) res);
        Summator = Parser::itos((int) res);
    }
    memory.push(op1, Summator);
}

void Processor::mulInt()
{
    LoadRegisters(I1, I2, "MULINT", "*");

    long long res = (long long)I1 * (long long)I2;

    OutRangeChecker(res, MULINT);

    omega_res((int)res);
    Summator = Parser::itos((int)res);
    memory.push(op1, Summator);
}

void Processor::divInt()
{
    LoadRegisters(I1, I2, "DIVINT", "/");

    if (I2 == 0) throw NULL_DIVIDE(saveRA, (int)CommandCode::DIVINT, op1, op2, op3);

    long long res = (long long)I1 / (long long)I2;

    OutRangeChecker(res, DIVINT);

    omega_res((int)res);
    Summator = Parser::itos((int)res);
    memory.push(op1, Summator);
}

void Processor::modInt()
{
    LoadRegisters(I1, I2, "MODINT", "%");

    if (I2 == 0) throw NULL_DIVIDE(saveRA, (int)CommandCode::MOD, op1, op2, op3);

    long long res = (long long)I1 % (long long)I2;

    OutRangeChecker(res, MOD);

    omega_res((int)res);
    Summator = Parser::itos((int)res);
    memory.push(op1, Summator);
}

void Processor::inFloat()
{
    float value;

    while (op2-- > 0)
    {
        bool ok = false;

        cout << "Input float to " << "<"<<op1<<">" << ">:";
        logFile << "Input float to "<< "<"<<op1<<">" << ":";

        while (!ok)
        {
            try {
                value = Parser::getTokenFloat();
                logFile << value << "\n";
                ok = true;
            }

            catch (Exception &err) {
                logFile << err.what() << "\nRewrite please:";
                cout << err.what() << "\nRewrite please:";
            }
        }

        memory.push(op1, Parser::ftos(value));
        op1 = (op1 + 1) % 512;
    }
}

void Processor::outFloat()
{
    while(op2-- > 0)
    {
        auto val = (float)(Parser::stold(memory.get(op1++)));
        logFile << "Float from " << "<"<<op1<<">" << ": " << val << "\n";
        cout << "Float from " << "<"<<op1<<">" << ": " << val << "\n";
    }
}

void Processor::addFloat()
{
    LoadRegisters(F1, F2, "ADDFLOAT", "+");
    long double res = F1 + F2;

    OutRangeChecker(res, ADDFLOAT);

    omega_res((float)res);
    Summator = Parser::ftos((float)res);
    memory.push(op1, Summator);
}

void Processor::subFloat()
{
    if (op2 == op3)
    {
        SubSelfSelf();
    }else
    {
        LoadRegisters(F1, F2, "SUBFLOAT", "-");
        long double res = F1 - F2;

        OutRangeChecker(res, SUBFLOAT);

        omega_res((float) res);
        Summator = Parser::ftos((float) res);
    }
    memory.push(op1, Summator);
}

void Processor::mulFloat()
{
    LoadRegisters(F1, F2, "MULFLOAT", "*");
    long double res = F1 * F2;

    OutRangeChecker(res, MULFLOAT);

    omega_res((float)res);
    Summator = Parser::ftos((float)res);
    memory.push(op1, Summator);
}

void Processor::divFloat()
{
    LoadRegisters(F1, F2, "DIVFLOAT", "/");

    if (F2 == 0) throw NULL_DIVIDE(saveRA, (int)CommandCode::DIVFLOAT, op1, op2, op3);

    long double res = F1 / F2;

    OutRangeChecker(res, DIVFLOAT);

    omega_res((float)res);
    Summator = Parser::ftos((float)res);
    memory.push(op1, Summator);
}

void Processor::intToFloat ()
{
    // int is always placed in float
    memory.push(op1, Parser::ftos((float)Parser::stoi(memory.get(op3))));
    logFile << "<"<<op1<<">" << " = " << "(float)" << "<"<<op3<<">" << "\n";
}

void Processor::floatToInt ()
{
    long double F = Parser::stold(memory.get(op3));

    if (F < minInt || F > maxInt)
    {
        Err = true;
        throw FTOIOutRange(saveRA, (int)CommandCode::FTOI, op1, op2, op3, F);
    }

    memory.push(op1, Parser::itos((int)F));
    logFile << "<"<<op1<<">" << " = " << "(float)" << "<"<<op3<<">" << "\n";
}

void Processor::unconditional ()
{
    RA = op2;
    logFile << "UNCOND: ";
    logFile << "Jump from " << saveRA << " to " << op2 << "\n";
}

void Processor::PR()
{
    logFile << "PR: " << "omega = " << omega;
    if (omega == 0)
    {
        RA = op2;
        logFile << ", Jump from " << saveRA << " to " << op2 << "\n";
    }
}

void Processor::PNR ()
{
    logFile << "PNR: " << "omega = " << omega;
    if (omega != 0)
    {
        RA = op2;
        logFile << ", Jump from " << saveRA << " to " << op2 << "\n";
    }
}

void Processor::PB ()
{
    logFile << "PB: " << "omega = " << omega;
    if (omega == 2)
    {
        RA = op2;
        logFile << ", Jump from " << saveRA << " to " << op2 << "\n";
    }
}

void Processor::PM ()
{
    logFile << "PM: " << "omega = " << omega;
    if (omega == 1)
    {
        RA = op2;
        logFile << ", Jump from " << saveRA << " to " << op2 << "\n";
    }
}

void Processor::PBR ()
{
    logFile << "PBR: " << "omega = " << omega;
    if (omega != 1)
    {
        RA = op2;
        logFile << ", Jump from " << saveRA << " to " << op2 << "\n";
    }
}

void Processor::PMR ()
{
    logFile << "PMR: " << "omega = " << omega;
    if (omega != 2)
    {
        RA = op2;
        logFile << ", Jump from " << saveRA << " to " << op2 << "\n";
    }
}

void Processor::just_if ()
{
    logFile << "IF: " << "omega = " << omega;
    switch (omega)
    {
        case 0:
            RA = op1;
            logFile << ", Jump from " << saveRA << " to " << op1 << "\n";
            break;
        case 1:
            RA = op2;
            logFile << ", Jump from " << saveRA << " to " << op2 << "\n";
            break;
        case 2:
            RA = op3;
            logFile << ", Jump from " << saveRA << " to " << op3 << "\n";
    }
}

void Processor::send()
{
    memory.push(op1, memory.get(op3));
}

bool Processor::tact()
{
    saveRA = RA;
    RK = memory.get(RA);
    RA = (RA + 1) % 512;

    Parser::cellParser(RK, RKcommand, op1, op2, op3);

    switch (RKcommand)
    {
        case CommandCode::ININT:
            inInt();
            break;
        case CommandCode::OUTINT:
            outInt();
            break;
        case CommandCode::ADDINT:
            addInt();
            break;
        case CommandCode::SUBINT:
            subInt();
            break;
        case CommandCode::MULINT:
            mulInt();
            break;
        case CommandCode::DIVINT:
            divInt();
            break;
        case CommandCode::MOD:
            modInt();
            break;
        case CommandCode::INFLOAT:
            inFloat();
            break;
        case CommandCode::OUTFLOAT:
            outFloat();
            break;
        case CommandCode::ADDFLOAT:
            addFloat();
            break;
        case CommandCode::SUBFLOAT:
            subFloat();
            break;
        case CommandCode::MULFLOAT:
            mulFloat();
            break;
        case CommandCode::DIVFLOAT:
            divFloat();
            break;
        case CommandCode::ITOF:
            intToFloat();
            break;
        case CommandCode::FTOI:
            floatToInt();
            break;
        case CommandCode::UNCOND:
            unconditional();
            break;
        case CommandCode::PR:
            PR();
            break;
        case CommandCode::PNR:
            PNR();
            break;
        case CommandCode::PB:
            PB();
            break;
        case CommandCode::PM:
            PM();
            break;
        case CommandCode::PBR:
            PBR();
            break;
        case CommandCode::PMR:
            PMR();
            break;
        case CommandCode::IF:
            just_if();
            break;
        case CommandCode::SEND:
            send();
            break;
        case CommandCode::END:
            return false;
        default:
            throw Bad_command(saveRA, (int)RKcommand, op1, op2, op3);
    }
    return true;
}

void Processor::main_process()
{
    try
    {
        while (!Err && iterations++ != max_iterations && tact());
    }

    catch (MemoryUndefined& err)
    {
        throw MemoryUndefined(saveRA, err.address);
    }
}

void Processor::set_max_iterations(int num)
{
    max_iterations = num;
}

string Processor::output_stat()
{
    string answer;
    answer += "\n-----------------------------------------------------\n";
    answer += "Register statistics:\n";
    answer +=  "RK      : ";
    answer += Parser::itos(saveRA, 3, 10) + " " + RK;
    answer += "\n";
    answer += "Pars RK : " + pars_obj.getCommandLexem(RKcommand) + " " +
              Parser::itos(op1, 3, 10) + " " +
              Parser::itos(op2, 3, 10) + " " +
              Parser::itos(op3, 3, 10);
    answer += "\n";
    answer += ("R1      : " + R1);
    answer += "\n";
    answer += ("R2      : " + R2);
    answer += "\n";
    answer += ("Summator: " + Summator);
    answer += "\n";
    answer += ("Omega   : ");
    answer += (char) ('0' + omega);
    answer += "\n";
    return answer;
}

void Processor::LoadRegisters(int &REG1, int &REG2, string command, string sign)
{
    R1 = memory.get(op2);
    R2 = memory.get(op3);
    REG1 = Parser::stoi(R1);
    REG2 = Parser::stoi(R2);

    logFile << command << ": " << "<"<<op1<<">" << " = "
            << "<"<<op2<<">" << "("<<REG1<<")"
            <<" " << sign << " "
            << "<"<<op3<<">" << "("<<REG2<<")";
}

void Processor::LoadRegisters(long double &REG1, long double &REG2, string command, string sign)
{
    R1 = memory.get(op2);
    R2 = memory.get(op3);
    REG1 = Parser::stold(R1);
    REG2 = Parser::stold(R2);

    logFile << command << ": " << "<"<<op1<<">" << " = "
            << "<"<<op2<<">" << "("<<REG1<<")"
            <<" " << sign << " "
            << "<"<<op3<<">" << "("<<REG2<<")";
}

void Processor::OutRangeChecker(long long res, CommandCode command)
{
    if (res < minInt || res > maxInt)
    {
        Err = true;
        throw MathOutRange(saveRA, (int)command, op1, op2, op3, I1, I2);
    }

    logFile << " = " << res << "\n";
}

void Processor::OutRangeChecker(long double res, CommandCode command)
{
    if (res != 0 && (abs(res) > maxFloat || abs(res) < minFloat))
    {
        Err = true;
        throw MathOutRange(saveRA, (int)command, op1, op2, op3, (float)F1, (float)F2);
    }

    logFile << " = " << res << "\n";
}

void Processor::SubSelfSelf()
{
    Summator = "00000000000000000000000000000000";
    R1       = "00000000000000000000000000000000";
    R2       = "00000000000000000000000000000000";
    omega    = 0;
}