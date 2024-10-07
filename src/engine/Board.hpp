#include <constants.hpp>
#include <list>

class Board {
    public:
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

    std::list<Board> generate_white_moves(int depth);
    std::list<Board> generate_black_moves(int depth);

    inline bool get_white_king_moved() const{return board_info & 0b10000000;};
    inline bool get_black_king_moved() const{return board_info & 0b01000000;};
    inline bool get_white_lrook_moved() const{return board_info & 0b00100000;};
    inline bool get_white_rrook_moved() const{return board_info & 0b00010000;};
    inline bool get_black_lrook_moved() const{return board_info & 0b00001000;};
    inline bool get_black_rrook_moved() const{return board_info & 0b00000100;};
    inline bool get_white_king_check() const{return board_info & 0b00000010;};
    inline bool get_white_king_check() const{return board_info & 0b00000001;};

    inline void set_white_king_moved() {board_info | 0b10000000;};
    inline void set_black_king_moved() {board_info | 0b01000000;};
    inline void set_white_lrook_moved() {board_info | 0b00100000;};
    inline void set_white_rrook_moved() {board_info | 0b00010000;};
    inline void set_black_lrook_moved() {board_info | 0b00001000;};
    inline void set_black_rrook_moved() {board_info | 0b00000100;};
    inline void set_white_king_check() {board_info | 0b00000010;};
    inline void set_white_king_check() {board_info | 0b00000001;};

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