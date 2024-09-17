#ifndef Board_HPP_
#define Board_HPP_

#include <array>
typedef uint64_t U64;


class Board {
    public:
    // constructors

    /**
     * creates a new empty board
     */
    Board() {
        bitboards = std::array<U64, 12>{};
    }

    // Board setup
    void boardSetup() {
        // set the starting pieces
    }

    // piece getters
    inline U64 getWhitePawns() const{return bitboards[0];}
	inline U64 getWhiteKnights() const{return bitboards[1];}
	inline U64 getWhiteBishops() const{return bitboards[2];}
	inline U64 getWhiteRooks() const{return bitboards[3];}
	inline U64 getWhiteQueens() const{return bitboards[4];}
	inline U64 getWhiteKing() const{return bitboards[5];}

	inline U64 getBlackPawns() const{return bitboards[6];}
	inline U64 getBlackKnights() const{return bitboards[7];}
	inline U64 getBlackBishops() const{return bitboards[8];}
	inline U64 getBlackRooks() const{return bitboards[9];}
	inline U64 getBlackQueens() const{return bitboards[10];}
	inline U64 getBlackKing() const{return bitboards[11];}


    private:
    /**
     * Board indices:
     * 0. white pawns
     * 1. white knights
     * 2. white bishops
     * 3. white rooks
     * 4. white queen
     * 5. white king
     * 6. black pawns
     * 7. black knights
     * 8. black bishops
     * 9. black rooks
     * 10. black queen
     * 11. black king
     */
    std::array<U64, 12> bitboards;
};

#endif /* Board_HPP_ */