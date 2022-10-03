#ifndef __KING__
#define __KING__

#include "../BoardAndPiece.hpp"

class King : public Piece {
    void add_if_valid(std::vector<Move> &moves, const Field &f, bool color) const;

    char castle_field_attacked() const;
public:
    std::vector<Move> get_possible_moves() const override;
};


#endif
