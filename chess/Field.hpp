#ifndef __FIELD__
#define __FIELD__

#include <iostream>
#include <string>
#include "../cpp-logger/Logger.hpp"

class Field {
    char _col;
    int _row;
public:
    Field();

    Field(const Field &f);

    Field(std::string field);

    Field(int i, int j);

    char get_col() const;

    int get_row() const;

    int get_i() const;

    int get_j() const;

    bool isValid() const;

    bool operator==(const Field &f) const;

    bool operator==(const std::string &f) const;

    void print() const;

    std::string to_string() const;
};

#endif