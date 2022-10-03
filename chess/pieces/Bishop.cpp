#include "Bishop.hpp"

std::vector<Move> Bishop::get_possible_moves() const {
    int i = get_i();
    int j = get_j();

    Logger::debug("Checking for Bishop on: " + _f.to_string());
    std::vector<Move> moves;
    
    bool color = get_color();
    char who = 'b' - color*32;

    for(int l = 0; l < 4; ++l) {
        for(int k = 1; k < 8; ++k) {
            Field f(l&2? i + k : i - k, l&1? j + k : j - k);
            if(f.isValid()) {
                if(_board->isEmpty(f)) {
                    moves.push_back(Move(who, false, _f, f));
                } else {
                    if(color == _board->isBlack(f)) {
                        moves.push_back(Move(who, true, _f, f));
                    }
                    break;
                }
            } else {
                break;
            }
        }
    }

    return moves;
}