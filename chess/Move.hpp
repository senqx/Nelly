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
    
	
	/// String representation move types
    static const char* move;
    static const char* capture;

    /// Create empty (fake) move to get same position but with opponent's move
    Move();

    Move(char who, bool isCapture, Field from, Field to);

    /// Return human-readable string representation of the move
    // (For example: e4, Ra1->a4, 0-0)
    std::string to_string() const;

    /// Is this move valid or fake move
    bool isValid() const;
};

#endif
