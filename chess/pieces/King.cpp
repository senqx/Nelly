#include "King.hpp"

void King::add_if_valid(std::list<Move> &moves, const Field &f,
    bool color) const
{
    if(f.isValid()) {
		if(_board->isEmpty(f)) {
            moves.push_back(Move('k' - color*32, false, _f, f));	
		} else {
			if(color == _board->isBlack(f)) {
	            moves.push_back(Move('k' - color*32, true, _f, f));	
			}
		}
    }
}

char King::castle_field_attacked() const {
    Logger::debug("Checking for castle field (King's nearby fields)\
        being under attack");
    bool color = get_color();
    // binary representation for King's side attack and Queen's => kq
    char attacked = 0;
    Board* b = _board->make_move(Move());
    std::list<Move> m = b->get_all_possible_moves_but_kings();
    for(auto it = m.begin(); it != m.end(); ++it) {
        if(it->to == Field(0 + color * 7, 5) ) {
            attacked |= 2;
        }
        if(it->to == Field(0 + color * 7, 3)) {
            attacked |= 1;
        }
    }
    delete b;
    return attacked;
}

std::list<Move> King::get_possible_moves() const {
    int j = get_j();
    int i = get_i();

    Logger::debug("Checking for King on: " + _f.to_string());
    std::list<Move> moves;
    
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

	if(c) {
    	char attacked = castle_field_attacked();
    	// King side
		if(c & (0b0010 << (color * 2))) {
        	f = Field(color * 7, 6);
        	if(_board->isEmpty(0 + color * 7, 5) &&
            	_board->isEmpty(f) && !(attacked & 2))
        	{
            	moves.push_back(Move('k' - color*32, false, _f, f));
        	}
    	}
		
		// Queen's side
    	if(c & (0b0001 << (color * 2))) {
        	f = Field(color * 7, 2);
        	if(_board->isEmpty(0 + color * 7, 3) &&
            	_board->isEmpty(f) && !(attacked & 1))
        	{
            	moves.push_back(Move('k' - color*32, false, _f, f));
        	}
    	}
	} else {
		Logger::debug(
			"No castle move available, so skipping castle field attack check");
	}
    
    return moves;
}
