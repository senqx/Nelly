#include "Pieces.hpp"
#include <vector>

std::vector<Move>
	Pawn::getValidMoves(Board* b, unsigned char pos) const noexcept
{
	bool isWhite = b->getVal(pos) == 'P';
	std::vector<Move> moves;

	if(isWhite) {
		// 
	}
	return moves;
}

std::vector<Move>
	Knight::getValidMoves(Board* b, unsigned char pos) const noexcept
{
	return std::vector<Move>(1);
}

std::vector<Move>
	Bishop::getValidMoves(Board* b, unsigned char pos) const noexcept
{
	return std::vector<Move>(1);
}

std::vector<Move>
	Rook::getValidMoves(Board* b, unsigned char pos) const noexcept
{
	return std::vector<Move>(1);
}

std::vector<Move>
	Queen::getValidMoves(Board* b, unsigned char pos) const noexcept
{
	return std::vector<Move>(1);
}

std::vector<Move>
	King::getValidMoves(Board* b, unsigned char pos) const noexcept
{
	return std::vector<Move>(1);
}
