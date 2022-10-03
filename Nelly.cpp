#include "Nelly.hpp"

Nelly::Nelly() : _board(nullptr) {}

void Nelly::load_board_from_fen(std::string FEN) {
	if(!_board) {
		_board = new Board();
	}
	_board->load_fen(FEN);
}

std::string Nelly::get_board_string() const {
	return _board->to_string();
}

std::string Nelly::get_fen() const {
	return _board->get_fen();
}

std::vector<Move> Nelly::get_valid_moves() const {
	return _board->get_valid_moves();
}

void Nelly::move(const Move &m) {
	Board* prev = _board;
	_board = _board->make_move(m);
	delete prev;
}

Nelly::~Nelly() {
	delete _board;
}