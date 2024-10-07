#include <cstdint>
#include <array>

typedef uint64_t U64;

enum pieces { 
    white_pawn, 
    white_knight, 
    white_bishop, 
    white_rook, 
    white_queen, 
    white_king,
    black_pawn,
    black_knight,
    black_bishop,
    black_rook,
    black_queen,
    black_king,
    empty
    };

// constants
const uint64_t RANK_1 = 0x00000000000000FF;
const uint64_t RANK_2 = 0x000000000000FF00;
const uint64_t RANK_3 = 0x0000000000FF0000;
const uint64_t RANK_4 = 0x00000000FF000000;
const uint64_t RANK_5 = 0x000000FF00000000;
const uint64_t RANK_6 = 0x0000FF0000000000;
const uint64_t RANK_7 = 0x00FF000000000000;
const uint64_t RANK_8 = 0xFF00000000000000;
const uint64_t FILE_A = 0x0101010101010101;
const uint64_t FILE_B = 0x0202020202020202;
const uint64_t FILE_C = 0x0404040404040404;
const uint64_t FILE_D = 0x0808080808080808;
const uint64_t FILE_E = 0x1010101010101010;
const uint64_t FILE_F = 0x2020202020202020;
const uint64_t FILE_G = 0x4040404040404040;
const uint64_t FILE_H = 0x8080808080808080;

const std::array<U64, 12> STARTING_POSITIONS = {
    // White pieces (first 6)
    0x000000000000FF00ULL,  // White Pawns (on Rank 2)
    0x0000000000000042ULL,  // White Knights (on b1 and g1)
    0x0000000000000024ULL,  // White Bishops (on c1 and f1)
    0x0000000000000081ULL,  // White Rooks (on a1 and h1)
    0x0000000000000008ULL,  // White Queen (on d1)
    0x0000000000000010ULL,  // White King (on e1)

    // Black pieces (last 6)
    0x00FF000000000000ULL,  // Black Pawns (on Rank 7)
    0x4200000000000000ULL,  // Black Knights (on b8 and g8)
    0x2400000000000000ULL,  // Black Bishops (on c8 and f8)
    0x8100000000000000ULL,  // Black Rooks (on a8 and h8)
    0x0800000000000000ULL,  // Black Queen (on d8)
    0x1000000000000000ULL   // Black King (on e8)
};
