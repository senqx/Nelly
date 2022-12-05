#include "Field.hpp"

Field::Field() : _col(0), _row(0) {}

Field::Field(const Field &f) {
    _col = f._col;
    _row = f._row;
}

Field::Field(std::string field) {
    if(field.size() != 2) {
        throw "Move needs 2 characters.";
    }
    
    _col = field[0];
    _row = field[1] - '0';
	
	// Not valid field (aka NULL state for Field)
    if(!(_col || _row )) {
        return;
    }

	// Other variations are illegal

    if(_col < 'a' || _col > 'h') {
        throw "Invalid col value. (Must be between [a-h])";
    }

    if(_row < 1 || _row > 8) {
        throw "Invalid row value. (Must be between [1-8])";
    }
}

Field::Field(int i, int j) {
    _col = 'a' + j;
    _row = 8 - i;
}

char Field::get_col() const {
    return _col;
}

int Field::get_row() const {
    return _row;
}

int Field::get_i() const {
    return 8 - _row;
}

int Field::get_j() const {
    return _col - 'a';
}

bool Field::isValid() const {
    return _col >= 'a' && _col <= 'h' && _row >= 1 && _row <= 8;
}

bool Field::operator==(const Field &f) const {
    return f._col == _col && f._row == _row;
}

bool Field::operator==(const std::string &f) const {
    if(f.size() != 2) {
        return false;
    }

    return (_col == f[0]) && (_row == f[1] - '0');
}

std::string Field::to_string() const {
    std::string s = " 0";
    s[0] = _col;
    s[1] += _row;
    return s;
}
