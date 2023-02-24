#ifndef __BOARD__
#define __BOARD__

#include <exception>
#include <iostream>
#include <string>
#include <cassert>

class FenException : public std::exception {
	const char* _msg;
public:
	FenException(const char* msg) 
		: _msg(msg) {}

	const char* what() const noexcept {
		return _msg;
	}
};

class Board {
public:
	static constexpr unsigned char SIZE = 8;

private:

	char _board[SIZE * SIZE];

	// Castle is available for
	// White: [Q]ueen-side [K]ing-side
	// Black: [q]ueen-side [k]ing-side
	char _QKqk;
	bool _isWhitesMove;
	unsigned char _enPass;
	unsigned char _halfMoves;
	unsigned short _fullMoves;

	// Place pieces based on FEN
	unsigned char place(const std::string& fen);

	// Get who's moving from FEN
	void whosMove(unsigned char& i, const std::string& fen);

	// Load Castles from FEN
	void loadCastles(unsigned char& i, const std::string& fen);
	
	// Load EnPass from FEN
	void loadEnPass(unsigned char& i, const std::string& fen);
	
	// Load Moves from FEN
	void loadMoves(unsigned char& i, const std::string& fen);

public:
	
	Board();

	void loadFen(const std::string&& fen =
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR") noexcept;

	char getVal(const unsigned char pos) const noexcept;
	
	void print() const noexcept;
};

#endif
