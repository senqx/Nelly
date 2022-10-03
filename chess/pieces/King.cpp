#include "King.hpp"

void King::add_if_valid(std::vector<Move> &moves, const Field &f, bool color) const {
    if(f.isValid()) {
        bool capture = (color == _board->isBlack(f));
        if(capture || _board->isEmpty(f)) {
            moves.push_back(Move('k' - color*32, capture, _f, f));
        }
    }
}

char King::castle_field_attacked() const {
    Logger::debug("Checking for castle field (King's nearby fields) being under attack");
    bool color = get_color();
    char attacked = 0;  // binary representation for King's side attack and Queen's => kq
    Board* b = _board->make_move(Move());
    std::vector<Move> m = b->get_all_possible_moves_but_kings();
    for(int i = 0; i < m.size(); ++i) {
        if(m[i].to == Field(0 + color * 7, 5) ) {
            attacked |= 2;
        }
        if(m[i].to == Field(0 + color * 7, 3)) {
            attacked |= 1;
        }
    }
    delete b;
    return attacked;
}

std::vector<Move> King::get_possible_moves() const {
    int j = get_j();
    int i = get_i();

    Logger::debug("Checking for King on: " + _f.to_string());
    std::vector<Move> moves;
    
    bool color = get_color();
    bool capture;

    Field f(i - 1, j - 1);
    add_if_valid(moves, f, color);

    f = Field(i - 1, j);
    add_if_valid(moves, f, color);

    f = Field(i - 1, j + 1);
    add_if_valid(moves, f, color);

    // ---

    f = Field(i, j - 1);
    add_if_valid(moves, f, color);

    f = Field(i, j + 1);
    add_if_valid(moves, f, color);

    // ---

    f = Field(i + 1, j - 1);
    add_if_valid(moves, f, color);

    f = Field(i + 1, j);
    add_if_valid(moves, f, color);

    f = Field(i + 1, j + 1);
    add_if_valid(moves, f, color);

    // Castle
    char c = _board->get_castles_binary();
    char attacked = castle_field_attacked();
    if(c & (0b0010 << (color * 2))) {
        f = Field(color * 7, 6);
        if(_board->isEmpty(0 + color * 7, 5) && _board->isEmpty(f) && !(attacked & 2)) {
            moves.push_back(Move('k' - color*32, false, _f, f));
        }
    }

    if(c & (0b0001 << (color * 2))) {
        f = Field(color * 7, 2);
        if(_board->isEmpty(0 + color * 7, 3) && _board->isEmpty(f) && !(attacked & 1)) {
            moves.push_back(Move('k' - color*32, false, _f, f));
        }
    }
    
    return moves;
}