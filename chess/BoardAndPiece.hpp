#ifndef __BOARD_AND_PIECES__
#define __BOARD_AND_PIECES__

#include <vector>
#include "Move.hpp"

class Board;
class Piece;

class Board {
    static const int size = 8;

    /// The board
    char** _board;  

    /// Additional FEN info (What move is it? Is there an En pass ...)
    int _halfmoves; // Move count without pawn advance or capture
    int _move;
    char _castles;  // 0 0 0 0 | The binary representation of Castles where the sequence is Q-K-q-k
    bool _isWhitesMove;
    Field _en_pass;

    /// List of all pieces on the board
    std::vector<Piece*> _pieces;

    /// A methood that only places pieces on the board in given order
    void placing(std::string FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

public:
    Board();

    Board(const Board &b);

    /// Create board from FEN
    Board(std::string FEN);

    void load_fen(std::string FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    std::string get_fen() const;

    /// Gives the char of field value ('0' - if empty, 'p' for black pawn, 'P' for white pawn ...)
    char get_val(int i, int j) const;

    char get_val(const Field &f) const;

    /// Get en pass field (If no en pass, Non-Valid Field is returned)
    Field get_en_pass() const;

    /// Get castle info
    char get_castles_binary() const;

    /// Check if on given field there is a BLACK piece
    bool isBlack(int i, int j) const;

    bool isBlack(const Field &f) const;

    /// Check if on given field there is a WHITE piece
    bool isWhite(int i, int j) const;

    bool isWhite(const Field &f) const;

    /// Check if the given field is EMPTY
    bool isEmpty(int i, int j) const;

    bool isEmpty(const Field &f) const;

    /// Check if en pass is available on the given field
    bool isEnPass(const Field &f) const;

    /// Make the given move, and return new board
    Board* make_move(const Move &m) const;

    /// Check if after the move we check ourselfs (impossible move check)
    bool isSelfCheck(const Move &m) const;

    /// Is check there check in this position?
    bool isCheck() const;

    /// Calculate all possible moves for current board (Valid or not)
    std::vector<Move> get_all_possible_moves() const;

    /// Calculate all possible moves for current board but king's moves
    std::vector<Move> get_all_possible_moves_but_kings() const;

    /// Return only Valid moves (Filter invalid moves)
    std::vector<Move> get_valid_moves() const;

    /// The visualisation of the board
    std::string to_string() const;

    ~Board();
};

class Piece {
protected:
    Field _f;
    bool _isWhite;
    Board* _board;
public:
    Piece();

    Piece(const Field &f);

    /// Create Piece (in HEAP) from given name ('p' for black pawn, 'P'' for white pawn, ...)
    static Piece* create_piece(char letter);

    /// Methood for all pieces to calculate their possible (valid or not) moves
    virtual std::vector<Move> get_possible_moves() const = 0;

    /// 0 - Black, 1 - White
    bool get_color() const;

    /// Get matrix index i
    int get_i() const;

    /// Get matrix index j
    int get_j() const;

    Field get_field() const;

    /// "Put" current piece on the give field
    void set_field(const Field &f);

    void set_field(int i, int j);

    /// Set the color of the piece
    void set_color(bool col);

    /// Set the board (pointer) for this piece
    void set_board(Board* board);

    virtual ~Piece();
};

#endif
