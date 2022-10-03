#include "Rook.hpp"

std::vector<Move> Rook::get_possible_moves() const {
    int j = get_j();
    int i = get_i();

    Logger::debug("Checking for Rook on: " + _f.to_string());
    std::vector<Move> moves;
    
    bool color = get_color();
    
    for(int l = 0; l < 2; ++l) {
        for(int k = 1; k < 8; ++k) {
            Field f(i + (l? k : -k), j);
            if(f.isValid()) {
                if(_board->isEmpty(f)) {
                    moves.push_back(Move('r' - color*32, false, _f, f));
                } else {
                    if(color == _board->isBlack(f)) {
                        moves.push_back(Move('r' - color*32, true, _f, f));
                    }
                    break;
                }
            } else {
                break;
            }
        }
    }

    for(int l = 0; l < 2; ++l) {
        for(int k = 1; k < 8; ++k) {
            Field f(i, j + (l? k: -k));
            if(f.isValid()) {
                if(_board->isEmpty(f)) {
                    moves.push_back(Move('r' - color*32, false, _f, f));
                } else {
                    if(color == _board->isBlack(f)) {
                        moves.push_back(Move('r' - color*32, true, _f, f));
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