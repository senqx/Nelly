#include "Queen.hpp"
#include "Rook.hpp"
#include "Bishop.hpp"

std::list<Move> Queen::get_possible_moves() const {

    Logger::debug("Checking for Queen on: " + _f.to_string());
    
    bool color = get_color();

    std::list<Move> moves;
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
        auto r_moves = r.get_possible_moves();
        moves.insert(moves.end(), r_moves.begin(), r_moves.end());
    }

    for(auto it = moves.begin(); it != moves.end(); ++it) {
        it->who = 'q' - color*32;
    }

    return moves;
}
