#include <iostream>

class Exceptions
{
public:
    std::string message = "Error!";
};

class Bad_cell : public Exceptions {};

class Out_range : public Bad_cell
{
public:
    int bad_index;
    explicit Out_range (int bad_index = -1, std::string message = "Bad index!")
    {
        this->bad_index = bad_index;
        this->message = move(message);
    }
};

class Bad_command : public Bad_cell
{
public:
    std::string what_is;
    explicit Bad_command (std::string what_is, std::string message = "Bad command!")
    {
        this->what_is = move(what_is);
        this->message = move(message);
    }
};

class Empty : public Bad_cell
{
public:
    explicit Empty (std::string message = "Empty operands in cell!")
    {
        this->message = move(message);
    }
};