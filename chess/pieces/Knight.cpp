#include "Knight.hpp"

void Knight::add_if_valid(std::list<Move> &moves, const Field &f,
    bool color) const
{
    char who = 'n' - color*32;
    if(f.isValid()) {
        if(_board->isEmpty(f)) {
            moves.push_back(Move(who, false, _f, f));
        } else {
            if(color == _board->isBlack(f)) {
                moves.push_back(Move(who, true, _f, f));
            }
        }
    }
}

std::list<Move> Knight::get_possible_moves() const {
    int i = get_i();
    int j = get_j();

    Logger::debug("Checking for Knight on: " + _f.to_string());
    std::list<Move> moves;
    
    bool color = get_color();
    bool capture;
    for(int d = 1; d < 3; ++d) {
        Field f(i - d, j + d - 3);
        add_if_valid(moves, f, color);
        
        f = Field(i + d, j + d - 3);
        add_if_valid(moves, f, color);

        f = Field(i - d, j - d + 3);
        add_if_valid(moves, f, color);
        
        f = Field(i + d, j - d + 3);
        add_if_valid(moves, f, color);
    }
    return moves;
}
