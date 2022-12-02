#include "Move.hpp"

const char* Move::move = "->";
const char* Move::capture = "x";

Move::Move() : who(0) {}

Move::Move(char who, bool isCapture, Field from, Field to)
    : who(who), isCapture(isCapture), isCheck(false), from(from), to(to) {}

std::string Move::to_string() const {
    if(who == 0) {
        Logger::error("Fake move has no string representation");
        return "INVALID MOVE: Fake empty move,\
            to get the same board but with other color perspective";
    }

    // Castle
    if(who == 'k' || who == 'K') {
        if(from == "e1" || from == "e8") {
            if(to == "g1" || to == "g8") {
                return "0-0";
            }
            if(to == "c1" || to == "c8") {
                return "0-0-0";
            }
        }   
    }

    if(who != 'p' && who != 'P') {
        std::string s = who + from.to_string() +
            (isCapture? capture : move) + to.to_string();
        if(isCheck) {
            s.push_back('+');
        }
        return s;
    }

    // If it is a pawn
    if(isCapture) {
        std::string s = "  ";
        s[0] = from.get_col();
        s[1] = to.get_col();
        if(isCheck) {
            s.push_back('+');
        }
        return s;
    }

    std::string s = to.to_string();
    if(isCheck) {
        s.push_back('+');
    }
    return s;
}

bool Move::isValid() const {
    return who;
}
