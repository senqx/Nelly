#include "Queen.hpp"
#include "Rook.hpp"
#include "Bishop.hpp"

std::vector<Move> Queen::get_possible_moves() const {

    Logger::debug("Checking for Queen on: " + _f.to_string());
    
    bool color = get_color();

    std::vector<Move> moves;
    {
        Bishop b;
        b.set_board(_board);
        b.set_color(color);
        b.set_field(_f);
        moves = b.get_possible_moves();
    }

    {
        Rook r;
        r.set_board(_board);
        r.set_color(color);
        r.set_field(_f);
        std::vector<Move> r_moves = r.get_possible_moves();
        moves.insert(moves.end(), r_moves.begin(), r_moves.end());
    }

    for(int i = 0; i < moves.size(); ++i) {
        moves[i].who = 'q' - color*32;
    }

    return moves;
}