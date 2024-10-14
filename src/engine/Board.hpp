#include <constants.hpp>
#include <list>

class Board {
    public:
    // piece existence
    const int piece_at_square(U64 rank, U64 file);
    // piece getters
    inline U64 get_white_pawns() const{return bitboards[0];}
	inline U64 get_white_knights() const{return bitboards[1];}
	inline U64 get_white_bishops() const{return bitboards[2];}
	inline U64 get_white_rooks() const{return bitboards[3];}
	inline U64 get_white_queens() const{return bitboards[4];}
	inline U64 get_white_king() const{return bitboards[5];}

	inline U64 get_black_pawns() const{return bitboards[6];}
	inline U64 get_black_knights() const{return bitboards[7];}
	inline U64 get_black_bishops() const{return bitboards[8];}
	inline U64 get_black_rooks() const{return bitboards[9];}
	inline U64 get_black_queens() const{return bitboards[10];}
	inline U64 get_black_king() const{return bitboards[11];}

    inline void set_white_pawns(U64 new_board) {bitboards[0] = new_board;}
	inline void set_white_knights(U64 new_board) {bitboards[1] = new_board;}
	inline void set_white_bishops(U64 new_board) {bitboards[2] = new_board;}
	inline void set_white_rooks(U64 new_board) {bitboards[3] = new_board;}
	inline void set_white_queens(U64 new_board) {bitboards[4] = new_board;}
	inline void set_white_king(U64 new_board) {bitboards[5] = new_board;}
	inline void set_black_pawns(U64 new_board) {bitboards[6] = new_board;}
	inline void set_black_knights(U64 new_board) {bitboards[7] = new_board;}
	inline void set_black_bishops(U64 new_board) {bitboards[8] = new_board;}
	inline void set_black_rooks(U64 new_board) {bitboards[9] = new_board;}
	inline void set_black_queens(U64 new_board) {bitboards[10] = new_board;}
	inline void set_black_king(U64 new_board) {bitboards[11] = new_board;}

    std::list<Board> generate_white_moves();
    std::list<Board> generate_black_moves();

    inline bool get_white_king_moved() const{return board_info & 0b10000000;};
    inline bool get_black_king_moved() const{return board_info & 0b01000000;};
    inline bool get_white_lrook_moved() const{return board_info & 0b00100000;};
    inline bool get_white_rrook_moved() const{return board_info & 0b00010000;};
    inline bool get_black_lrook_moved() const{return board_info & 0b00001000;};
    inline bool get_black_rrook_moved() const{return board_info & 0b00000100;};
    inline bool get_white_king_check() const{return board_info & 0b00000010;};
    inline bool get_white_king_check() const{return board_info & 0b00000001;};

    inline void set_white_king_moved(bool toggle) {
        if (toggle) {
            board_info |= 0b10000000;
        } else {
            board_info &= 0b01111111;
        }
    };
    inline void set_black_king_moved(bool toggle) {
        if (toggle) {
            board_info |= 0b01000000;
        } else {
            board_info &= 0b10111111;
        }
    };
    inline void set_white_lrook_moved(bool toggle) {
        if (toggle) {
            board_info |= 0b00100000;
        } else {
            board_info &= 0b11011111;
        }
    };
    inline void set_white_rrook_moved(bool toggle) {
        if (toggle) {
            board_info |= 0b00010000;
        } else {
            board_info &= 0b11101111;
        }
    };
    inline void set_black_lrook_moved(bool toggle) {
        if (toggle) {
            board_info |= 0b00001000;
        } else {
            board_info &= 0b11110111;
        }
    };
    inline void set_black_rrook_moved(bool toggle) {
        if (toggle) {
            board_info |= 0b00000100;
        } else {
            board_info &= 0b11111011;
        }
    };
    inline void set_white_king_check(bool toggle) {
        if (toggle) {
            board_info |= 0b00000010;
        } else {
            board_info &= 0b11111101;
        }
    };
    inline void set_white_king_check(bool toggle) {
        if (toggle) {
            board_info |= 0b00000001;
        } else {
            board_info &= 0b11111110;
        }
    };
    void move_piece(U64 prev_rank, U64 prev_file, U64 next_rank, U64 next_file);
    void remove_piece(U64 rank, U64 file);
    private:
    int score;
    std::array<U64, 12> bitboards;
    /**
     * each bit has the following values:
     * 0 white king has moved
     * 1 black king has moved
     * 2 white l rook has moved
     * 3 white r rook has moved
     * 4 black l rook has moved
     * 5 black r rook has moved
     * 6 white king under check
     * 7 black king under check
     */
    unsigned char board_info = 0;
};