#include "Chess.hpp"

Board::Board() : _en_pass(), _halfmoves(0), _move(1), _castles(0b1111),
    _isWhitesMove(true)
{
    Logger::debug("Creating empty board");

    _board = new char*[size];
    for(int i = 0; i < size; ++i) {
        _board[i] = new char[size];
    }

    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++j) {
            _board[i][j] = '0';
        }
    }
}

Board::Board(const Board &b) : _en_pass(b._en_pass), _halfmoves(b._halfmoves),
    _move(b._move), _castles(b._castles), _isWhitesMove(b._isWhitesMove),
    _pieces()
{
    Logger::debug("Copying board");

    _board = new char*[size];
    for(int i = 0; i < size; ++i) {
        _board[i] = new char[size];
    }

    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++j) {
            _board[i][j] = b._board[i][j];
            Piece* p = Piece::create_piece(_board[i][j]);
            if(p) {
                p->set_board(this);
                p->set_field(Field(i, j));
                _pieces.push_back(p);
            }
        }
    }
}

Board::Board(std::string FEN) {
    _board = new char*[size];
    for(int i = 0; i < size; ++i) {
        _board[i] = new char[size];
    }

    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++j) {
            _board[i][j] = '0';
        }
    }
    load_fen(FEN);
}

void Board::placing(std::string FEN) {
    int i = 0, j = 0;
    for (size_t index = 0; index < FEN.size(); ++index) {
        if (i > 7) {
            Logger::error("Wrong input");
            placing();
            return;
        }

        Piece* p;
        switch (FEN[index]) {
            case 'p':
            case 'n':
            case 'b':
            case 'r':
            case 'q':
            case 'k':
            case 'P':
            case 'N':
            case 'B':
            case 'R':
            case 'Q':
            case 'K':
                _board[i][j] = FEN[index];
                p = Piece::create_piece(FEN[index]);
                if(p) {
                    p->set_board(this);
                    p->set_field(Field(i, j));
                    _pieces.push_back(p);
                }
                ++j;
                break;
            case '/':
                ++i;
                j = 0;
                break;
            case '8':
            case '7':
            case '6':
            case '5':
            case '4':
            case '3':
            case '2':
            case '1':
                j += (FEN[index] - '0');
                break;
            default:
                Logger::error("Wrong character!");
                placing();
                return;
        }
    }
    Logger::debug("Placing successfully finished!");
}

void Board::load_fen(std::string FEN) {
    Logger::info("Loading FEN: " + FEN);

    int i = 0;

    // Placing
    std::string s_placing;
    while(FEN[i] != ' ') {
        s_placing.push_back(FEN[i]);
        ++i;
        if(i == FEN.size()) {
            Logger::error("Wrong FEN");
            load_fen();
            return;
        }
    }
    ++i;

    Logger::debug("Placing: " + s_placing);

    // Who's move it is?
    char now_moving = FEN[i];
    if(now_moving != 'w' && now_moving != 'b') {
        Logger::error("Wrong FEN");
        load_fen();
        return;
    }
    ++i;
    ++i;

    std::string nm = (now_moving == 'w')? "White" : "Black";
    Logger::debug("Now moving: " + nm);

    // Castle
    std::string castle;
    if(FEN[i] == '-') {
        ++i;
    } else {
        while(FEN[i] != ' ') {
            castle.push_back(FEN[i]);
            ++i;
        }
        }
    ++i;

    Logger::debug("Castle: " + castle);

    // En Passant
    if(FEN[i] == '-') {
        ++i;
    } else {
        std::string field;
        field.push_back(FEN[i]);
        ++i;
        field.push_back(FEN[i]);
        _en_pass = field;
    }
    ++i;

    if(_en_pass.isValid()) {
        Logger::debug("En passant: " + _en_pass.to_string());
    } else {
        Logger::debug("En passant: None");
    }

    // Halfmoves
    std::string tmp;
    while(FEN[i] != ' ') {
        tmp.push_back(FEN[i]);
        ++i;
    }
    ++i;
    Logger::debug("Halfmoves: " + tmp);
    int halfmoves = std::stoi(tmp);

    tmp.clear();
    while(i < FEN.size() && FEN[i] != ' ') {
        tmp.push_back(FEN[i]);
        ++i;
    }
    Logger::debug("Move N: " + tmp);
    int move = std::stoi(tmp);

    _move = move;
    _halfmoves = halfmoves;
    _isWhitesMove = now_moving == 'w';

    _castles = 0;

    for(int i = 0; i < castle.size(); ++i) {
        switch(castle[i]) {
            case 'K':
                _castles |= 0b1000;
                break;
            case 'Q':
                _castles |= 0b0100;
                break;
            case 'k':
                _castles |= 0b0010;
                break;
            case 'q':
                _castles |= 0b0001;
                break;
            default:
                Logger::error("Wrong FEN, so default is loaded instead");
                load_fen();
                return;
        }
    }

    placing(s_placing);
}

std::string Board::get_fen() const {
    std::string fen;
    for(int i = 0; i < 8; ++i) {
        int pass = 0;
        for(int j = 0; j < 8; ++j) {
            if(_board[i][j] != '0') {
                if(pass) {
                    fen.push_back(pass + '0');
                    pass = 0;
                }
                fen.push_back(_board[i][j]);
            } else {
                ++pass;
            }
        }

        if(pass) {
            fen.push_back(pass + '0');
        }

        fen.push_back('/');
    }

    fen[fen.size() - 1] = ' ';
    fen.push_back(_isWhitesMove? 'w' : 'b');
    fen.push_back(' ');

    if(_castles) {
        if(_castles & 0b1000) {
            fen.push_back('K');
        }

        if(_castles & 0b0100) {
            fen.push_back('Q');
        }

        if(_castles & 0b0010) {
            fen.push_back('k');
        }

        if(_castles & 0b0001) {
            fen.push_back('q');
        }
        
    } else {
        fen.push_back('-');
    }
    fen.push_back(' ');

    if(_en_pass.isValid()) {
        fen += _en_pass.to_string();
    } else {
        fen.push_back('-');
    }
    fen.push_back(' ');
    fen += std::to_string(_halfmoves);
    fen.push_back(' ');
    fen += std::to_string(_move);

    return fen;
}

char Board::get_val(int i, int j) const {
    return _board[i][j];
}

char Board::get_val(const Field &f) const {
    return _board[f.get_i()][f.get_j()];
}

Field Board::get_en_pass() const {
    return _en_pass;
}

char Board::get_castles_binary() const {
    return _castles;
}

bool Board::isWhite(int i, int j) const {
    return _board[i][j] > 'A' && _board[i][j] < 'Z';
}

bool Board::isBlack(int i, int j) const {
    return _board[i][j] > 'a' && _board[i][j] < 'z';
}

bool Board::isWhite(const Field &f) const {
    int i = f.get_i();
    int j = f.get_j();
    return _board[i][j] > 'A' && _board[i][j] < 'Z';
}

bool Board::isBlack(const Field &f) const {
    int i = f.get_i();
    int j = f.get_j();
    return _board[i][j] > 'a' && _board[i][j] < 'z';
}

bool Board::isEmpty(int i, int j) const {
    return _board[i][j] == '0';
}

bool Board::isEmpty(const Field &f) const {
    int i = f.get_i();
    int j = f.get_j();
    return _board[i][j] == '0';
}

bool Board::isEnPass(const Field &f) const {
    return _en_pass == f;
}

Board* Board::make_move(const Move &m) const {
    Board* b = new Board(*this);
    b->_isWhitesMove ^= 1;
    
    // If it was fake move, return the same position, but with opponent's move
    if(!m.isValid()) {
        return b;
    }
    
    Logger::debug("Making move: " + m.to_string());
    ++b->_move;

    // halfmoves counter
    if(m.isCapture || m.who == 'P' || m.who == 'p') {
        b->_halfmoves = 0;
    } else {
        ++b->_halfmoves;
    }

    // Castle rules
    if(m.who == 'K' || m.who == 'k') {
        if(m.from == "e1") {
            b->_castles &= 0b0011;
            if(m.to == "g1") {
                b->_board[7][5] = 'R';
                b->_board[7][7] = '0';
            } else if (m.to == "c1") {
                b->_board[7][3] = 'R';
                b->_board[7][0] = '0';
            }
        } else if(m.from == "e8") {
            b->_castles &= 0b1100;
            if(m.to == "g8") {
                b->_board[0][5] = 'r';
                b->_board[0][7] = '0';
            } else if (m.to == "c8") {
                b->_board[0][3] = 'r';
                b->_board[0][0] = '0';
            }
        }
        
    } else if(m.who == 'R' || m.who == 'r') {
        if(m.from == "a1") {
            b->_castles &= 0b1110;
        } else if(m.from == "h1") {
            b->_castles &= 0b1101;
        } else if(m.from == "a8") {
            b->_castles &= 0b1011;
        } else if(m.from == "h8") {
            b->_castles &= 0b0111;
        }
    }

    if(m.isCapture) {
        for(int i = 0; i < b->_pieces.size(); ++i) {
            if(m.to == b->_pieces[i]->get_field()) {
                delete b->_pieces[i];
                b->_pieces.erase(b->_pieces.begin() + i);
                break;
            }
        }
    }

    b->_board[m.to.get_i()][m.to.get_j()] = m.who;
    b->_board[m.from.get_i()][m.from.get_j()] = '0';

    return b;
}

bool Board::isSelfCheck(const Move &m) const {
    Logger::debug("Checking for self-check");
    Board* tmp = make_move(m);
    std::vector<Move> moves = tmp->get_all_possible_moves();
    char king = 'K' + tmp->_isWhitesMove*32;
    for(int i = 0; i < moves.size(); ++i) {
        if(tmp->get_val(moves[i].to) == king) {
            delete tmp;
            Logger::debug("Last move is self check");
            return true;
        }
    }
    delete tmp;
    Logger::debug("No self check found");
    return false;    
}

bool Board::isCheck() const {
    return isSelfCheck(Move());
}

std::vector<Move> Board::get_all_possible_moves() const {
    std::vector<Move> moves;
    for(int i = 0; i < _pieces.size(); ++i) {
        if(_pieces[i] && (_pieces[i]->get_color() == _isWhitesMove)) {
            std::vector<Move> movs = _pieces[i]->get_possible_moves();
            Logger::debug("Moves count is: " + std::to_string(movs.size()));
            moves.insert(moves.end(), movs.begin(), movs.end());
        }
    }
    return moves;
}

std::vector<Move> Board::get_all_possible_moves_but_kings() const {
    std::vector<Move> moves;
    for(int i = 0; i < _pieces.size(); ++i) {
        if(_pieces[i] && (get_val(_pieces[i]->get_field()) != 'K' &&
            get_val(_pieces[i]->get_field()) != 'k') &&
            (_pieces[i]->get_color() == _isWhitesMove))
        {
            std::vector<Move> movs = _pieces[i]->get_possible_moves();
            Logger::debug("Moves count without King is: " +
                std::to_string(movs.size()));
            moves.insert(moves.end(), movs.begin(), movs.end());
        }
    }
    return moves;
}

std::vector<Move> Board::get_valid_moves() const {
    std::vector<Move> m = get_all_possible_moves();
    std::vector<Move> valid_moves(m.size());

    int j = 0;
    for(int i = 0; i < m.size(); ++i) {
        if(!isSelfCheck(m[i])) {
            valid_moves[j] = m[i];
            ++j;
        }
    }

    valid_moves.resize(j);
    return valid_moves;
}

std::string Board::to_string() const {
    std::string s =
"/= = = = = = = = =\\\n\
| 0 0 0 0 0 0 0 0 |\n\
| 0 0 0 0 0 0 0 0 |\n\
| 0 0 0 0 0 0 0 0 |\n\
| 0 0 0 0 0 0 0 0 |\n\
| 0 0 0 0 0 0 0 0 |\n\
| 0 0 0 0 0 0 0 0 |\n\
| 0 0 0 0 0 0 0 0 |\n\
| 0 0 0 0 0 0 0 0 |\n\
\\= = = = = = = = =/";
    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++j) {
            s[i*20 + j*2 + 22] = _board[i][j];
        }
    }
    return s;
}

Board::~Board() {
    Logger::debug("Deleting pieces and board");

    for(int i = 0; i < _pieces.size(); ++i) {
        delete _pieces[i];
    }

    for(int i = 0; i < size; ++i) {
        delete [] _board[i];
    }
    delete [] _board;
}
