#include "Board.hpp"
#include <list>
#include <algorithm>

const int Board::piece_at_square(U64 rank, U64 file) {
    for (auto i = 0; i < 12; ++i) {
        if (bitboards[i] & rank & file) {
            return i;
        }
    }
    return empty;
}

void Board::move_piece(U64 prev_rank, U64 prev_file, U64 next_rank, U64 next_file) {
    int piece = piece_at_square(prev_rank, prev_file);
    bitboards[piece] &= ~prev_rank & ~prev_file;
    bitboards[piece] |= next_rank & next_file;
}

void Board::remove_piece(U64 rank, U64 file) {
    int piece = piece_at_square(rank, file);
    bitboards[piece] &= ~rank & ~file;
}

std::list<Board> Board::generate_white_moves() {
    auto possible_moves = std::list<Board>{};
    if (!get_white_king_moved() && !get_white_king_check()) {
        if (!get_white_lrook_moved()) {
            // check rank 1 and files bcd
            if (
                piece_at_square(RANK_1, FILE_B) == empty && 
                piece_at_square(RANK_1, FILE_C) == empty && 
                piece_at_square(RANK_1, FILE_D) == empty
                ) {
                    auto next_move = this;
                    next_move->set_white_king_moved(true);
                    next_move->set_white_lrook_moved(true);
                    // move king and rook
                    move_piece(RANK_1, FILE_A, RANK_1, FILE_D);
                    move_piece(RANK_1, FILE_E, RANK_1, FILE_C);
            }
        }
        if (!get_white_rrook_moved()) {
            // check rank 1 and files fg
            if (
                piece_at_square(RANK_1, FILE_F) == empty && 
                piece_at_square(RANK_1, FILE_G) == empty
            ) {
                auto next_move = this;
                next_move->set_white_king_moved(true);
                next_move->set_white_rrook_moved(true);
                // king can castle to right
                move_piece(RANK_1, FILE_H, RANK_1, FILE_F);
                move_piece(RANK_1, FILE_E, RANK_1, FILE_G);
            }
        }
    }
    // do pawn movements (en passant and promotion) (promotions only become knights or queens)
    // do king, knight, 

}