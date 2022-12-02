#ifndef __FIELD__
#define __FIELD__

#include <iostream>
#include <string>
#include "../cpp-logger/logger.hpp"

class Field {
    char _col;
    int _row;
public:
    Field();

    Field(const Field &f);

    /// Create field from given string (For example: Field f = "e4")
    Field(std::string field);

    /// Create filed from matrix coordinates
    Field(int i, int j);

    /// Get column (a, b, c, ... h)
    char get_col() const;

    /// Get row (1, 2, 3, ... 8)
    int get_row() const;

    /// Get matrix coordinate i (rows) -> (0, 1, 2, ... 7)
    int get_i() const;

    /// Get matrix coordinate j (cols) -> (0, 1, 2, ... 7)
    int get_j() const;

    /// Is the given field a valid chess field?
    // (For example: e4 -> true, a9 -> false)
    bool isValid() const;

    bool operator==(const Field &f) const;

    bool operator==(const std::string &f) const;

    /// Common string representation of field (For example: "e4")
    std::string to_string() const;
};

#endif
