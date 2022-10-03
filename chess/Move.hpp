#ifndef __MOVE__
#define __MOVE__

#include <iostream>
#include "Field.hpp"

struct Move {
    char who;
    bool isCapture;
    bool isCheck;
    Field from;
    Field to;
public:
    static const char*  move;
    static const char* capture;

    Move();

    Move(char who, bool isCapture, Field from, Field to);

    std::string to_string() const;

    bool isValid() const;
};

#endif