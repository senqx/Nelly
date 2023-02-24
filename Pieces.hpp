#ifndef __PIECES__
#define __PIECES__

#include <vector>
#include "Board.hpp"

struct Move {
	unsigned char from;
	unsigned char to;
	bool isCheck;

	Move() : from(-1), to(-1), isCheck(false) {}

	Move(unsigned char from, unsigned char to, bool isCheck)
		: from(from), to(to), isCheck(isCheck) {}
};

class Piece {
public:
	virtual std::vector<Move> 
		getValidMoves(Board* b, unsigned char pos) const noexcept = 0;
};

class Pawn : public Piece {
	virtual std::vector<Move>
		getValidMoves(Board* b, unsigned char pos) const noexcept override;
};

class Knight : public Piece {
	virtual std::vector<Move>
		getValidMoves(Board* b, unsigned char pos) const noexcept override;
};

class Bishop : public Piece {
	virtual std::vector<Move>
		getValidMoves(Board* b, unsigned char pos) const noexcept override;
};

class Rook : public Piece {
	virtual std::vector<Move>
		getValidMoves(Board* b, unsigned char pos) const noexcept override;
};

class Queen : public Piece {
	virtual std::vector<Move>
		getValidMoves(Board* b, unsigned char pos) const noexcept override;
};

class King : public Piece {
	virtual std::vector<Move>
		getValidMoves(Board* b, unsigned char pos) const noexcept override;
};

#endif
