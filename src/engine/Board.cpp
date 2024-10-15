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

// this generates pseudo-legal moves, illegal moves will be caught in decision trees and are irrelevent.
std::list<Board> Board::generate_white_moves() {
    U64 white_occupied = 0;
    U64 black_occupied = 0;
    auto possible_moves = std::list<Board>{};

    // set occupied bits
    for (auto i = 0; i < 6; ++i) {
        white_occupied |= bitboards[i];
    }
    for (auto i = 6; i < 12; ++i) {
        black_occupied |= bitboards[i];
    }
    /**
     * Castling moves
     * - change move_piece to bit manipulation for greater speed
     */
    if (!get_white_king_moved() && !get_white_king_check()) {
        if (!get_white_lrook_moved()) {
            // check rank 1 and files bcd
            if (
                piece_at_square(RANK_1, FILE_B) == empty && 
                piece_at_square(RANK_1, FILE_C) == empty && 
                piece_at_square(RANK_1, FILE_D) == empty
                ) {
                    auto next_move = *this;
                    next_move.set_white_king_moved(true);
                    next_move.set_white_lrook_moved(true);
                    // move king and rook
                    move_piece(RANK_1, FILE_A, RANK_1, FILE_D);
                    move_piece(RANK_1, FILE_E, RANK_1, FILE_C);
                    possible_moves.push_back(next_move);
            }
        }
        if (!get_white_rrook_moved()) {
            // check rank 1 and files fg
            if (
                piece_at_square(RANK_1, FILE_F) == empty && 
                piece_at_square(RANK_1, FILE_G) == empty
            ) {
                auto next_move = *this;
                next_move.set_white_king_moved(true);
                next_move.set_white_rrook_moved(true);
                // king can castle to right
                move_piece(RANK_1, FILE_H, RANK_1, FILE_F);
                move_piece(RANK_1, FILE_E, RANK_1, FILE_G);
                possible_moves.push_back(next_move);
            }
        }
    }
    // do pawn movements
    // single push, includes promotions
    U64 single_push = (get_white_pawns() << 8) & (~(white_occupied | black_occupied));
    bool promotion = false;
    while (single_push) {
        auto next_move = *this;
        auto pawn_move = single_push & -single_push;
        if (pawn_move & RANK_8) promotion = true;
        next_move.set_white_pawns(get_white_pawns() ^ (pawn_move >> 8));
        if (promotion) {
            auto knight_alternative = next_move;
            knight_alternative.set_white_knights(knight_alternative.get_white_knights() | pawn_move);
            possible_moves.push_back(knight_alternative);
            next_move.set_white_queens(next_move.get_white_queens() | pawn_move);
        } else {
            next_move.set_white_pawns(next_move.get_white_pawns() | pawn_move);
        }
        // add the next move to possible moves
        possible_moves.push_back(next_move);
        single_push &= -single_push;
        promotion = false;
    }
    // double push
    // rank 2 is only rank to be considered
    U64 double_push = (get_white_pawns() << 16) & (~(white_occupied | black_occupied)) & (~((white_occupied | black_occupied) << 8));
    while (double_push) {
        auto next_move = *this;
        auto pawn_move = double_push & -double_push;
        next_move.set_white_pawns(get_white_pawns() ^ (pawn_move >> 16));
        next_move.set_white_pawns(next_move.get_white_pawns() | pawn_move);
        possible_moves.push_back(next_move);
        double_push &= -double_push;
    }
    // pawn attack
    U64 left_pawn_attack = (get_white_pawns() << 9) & NOT_FILE_A & black_occupied;
    while (left_pawn_attack) {
        auto next_move = *this;
        auto pawn_move = left_pawn_attack & -left_pawn_attack;
        if (pawn_move & RANK_8) promotion = true;
        next_move.set_white_pawns(get_white_pawns() ^ (pawn_move >> 9));
        if (promotion) {
            auto knight_alternative = next_move;
            knight_alternative.set_white_knights(knight_alternative.get_white_knights() | pawn_move);
            possible_moves.push_back(knight_alternative);
            next_move.set_white_queens(next_move.get_white_queens() | pawn_move);
        } else {
            next_move.set_white_pawns(next_move.get_white_pawns() | pawn_move);
        }
        possible_moves.push_back(next_move);
        left_pawn_attack &= -left_pawn_attack;
        promotion = false;
        
    }
    U64 right_pawn_attack = (get_white_pawns() << 7) & NOT_FILE_H & black_occupied;
    while (right_pawn_attack) {
        auto next_move = *this;
        auto pawn_move = right_pawn_attack & -right_pawn_attack;
        if (pawn_move & RANK_8) promotion = true;
        next_move.set_white_pawns(get_white_pawns() ^ (pawn_move >> 7));
        if (promotion) {
            auto knight_alternative = next_move;
            knight_alternative.set_white_knights(knight_alternative.get_white_knights() | pawn_move);
            possible_moves.push_back(knight_alternative);
            next_move.set_white_queens(next_move.get_white_queens() | pawn_move);
        } else {
            next_move.set_white_pawns(next_move.get_white_pawns() | pawn_move);
        }
        possible_moves.push_back(next_move);
        right_pawn_attack &= -right_pawn_attack;
        promotion = false;
    }
    // en passant
    
    // do king, knight, 

}