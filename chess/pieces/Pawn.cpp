#include "Pawn.hpp"

std::vector<Move> Pawn::get_possible_moves() const {
    int j = get_j();
    int i = get_i();

    Logger::debug("Checking for Pawn on: " + _f.to_string());
    std::vector<Move> moves;
    
    bool color = get_color();
    char who = 'p' - color*32;

    Field f(i + 1 - 2*color, j - 1);
    if(f.isValid() && !_board->isEmpty(f) && ((color == _board->isBlack(f)) || _board->isEnPass(f))) {
        moves.push_back(Move(who, true, _f, f));
    }

    f = Field(i + 1 - 2*color, j + 1);
    if(f.isValid() && !_board->isEmpty(f) && ((color == _board->isBlack(f)) || _board->isEnPass(f))) {
        moves.push_back(Move(who, true, _f, f));
    }

    f = Field(i + 1 - 2*color, j);
    if(f.isValid() && _board->isEmpty(f)) {
        moves.push_back(Move(who, false, _f, f));

        f = Field(3 + color, j);
        if(i == 1 + 5*color && _board->isEmpty(f)) {
            moves.push_back(Move(who, false, _f, f));
        }
    }
    
    return moves;
}