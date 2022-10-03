#include "BoardAndPiece.hpp"
#include "Chess.hpp"

Piece::Piece()  : _f(), _isWhite(), _board(nullptr) {}

Piece::Piece(const Field &f) : _f(f), _isWhite(), _board(nullptr) {}

Piece* Piece::create_piece(char letter) {
    Piece* p = nullptr;
    switch(letter) {
        case 'p':
            p = new Pawn();
            break;
        case 'P':
            p = new Pawn();
            p->set_color(1);
            break;
        case 'n':
            p = new Knight();
            break;
        case 'N':
            p = new Knight();
            p->set_color(1);
            break;
        case 'b':
            p = new Bishop();
            break;
        case 'B':
            p = new Bishop();
            p->set_color(1);
            break;
        case 'r':
            p = new Rook();
            break;
        case 'R':
            p = new Rook();
            p->set_color(1);
            break;
        case 'q':
            p = new Queen();
            break;
        case 'Q':
            p = new Queen();
            p->set_color(1);
            break;
        case 'k':
            p = new King();
            break;
        case 'K':
            p = new King();
            p->set_color(1);
            break;
        case '0':
            break;
        default:
            Logger::error("Wrong piece letter");
    }

    return p;
}

bool Piece::get_color() const {
    return _isWhite;
}

int Piece::get_i() const {
    return _f.get_i();
}

int Piece::get_j() const {
    return _f.get_j();
}

Field Piece::get_field() const {
    return _f;
}

void Piece::set_field(const Field &f) {
    _f = f;
}

void Piece::set_field(int i, int j) {
    _f = Field(i, j);
}

void Piece::set_color(bool col) {
    _isWhite = col;
}

void Piece::set_board(Board* board) {
    _board = board;
}

Piece::~Piece() {}
