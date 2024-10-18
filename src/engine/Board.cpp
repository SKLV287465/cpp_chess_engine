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
        single_push ^= pawn_move;
        promotion = false;
    }
    // double push
    // rank 2 is only rank to be considered
    U64 double_push = (get_white_pawns() << 16) & (~(white_occupied | black_occupied)) & (~((white_occupied | black_occupied) << 8));
    while (double_push) {
        auto next_move = *this;
        auto pawn_move = double_push & -double_push;
        // set the en passant flag
        next_move.enpassant_w |= pawn_move >> 24;
        next_move.set_white_pawns(get_white_pawns() ^ (pawn_move >> 16));
        next_move.set_white_pawns(next_move.get_white_pawns() | pawn_move);
        possible_moves.push_back(next_move);
        double_push ^= pawn_move;
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
        left_pawn_attack ^= pawn_move;
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
        right_pawn_attack ^= pawn_move;
        promotion = false;
    }
    // en passant
    U64 left_enpassant = (get_white_pawns() << 9) & NOT_FILE_A & ~(white_occupied | black_occupied);
    // should check that there are black pawns behind the moves
    for (auto file : FILES) {
        if (!(file & left_enpassant)) continue;
        if (!(enpassant_b & file)) {
            left_enpassant ^ (file & RANK_5);
        } else if (!(get_black_pawns() & file & RANK_5)){
            left_enpassant ^ (file & RANK_5);
        }
    }
    while (left_enpassant) {
        auto next_move = *this;
        auto pawn_move = left_enpassant & -left_enpassant;
        // remove black pawn
        next_move.set_black_pawns(get_black_pawns() ^ (pawn_move >> 8));
        // change white pawns
        next_move.set_white_pawns((get_white_pawns() | pawn_move) ^ (pawn_move >> 9));
        possible_moves.push_back(next_move);
        left_enpassant ^= pawn_move;
    }
    U64 right_enpassant = (get_white_pawns() << 7) & NOT_FILE_H & ~(white_occupied | black_occupied);
    for (auto file : FILES) {
        if (!(file & right_enpassant)) continue;
        if (!(enpassant_b & file)) {
            right_enpassant ^ (file & RANK_5);
        } else if (!(get_black_pawns() & file & RANK_5)){
            right_enpassant ^ (file & RANK_5);
        }
    }
    while (right_enpassant) {
        auto next_move = *this;
        auto pawn_move = right_enpassant & -right_enpassant;
        // remove black pawn
        next_move.set_black_pawns(get_black_pawns() ^ (pawn_move >> 8));
        // change white pawns
        next_move.set_white_pawns((get_white_pawns() | pawn_move) ^ (pawn_move >> 7));
        possible_moves.push_back(next_move);
        right_enpassant ^= pawn_move;
    }
    // knight
    U64 original_knight_positions = get_white_knights();
    const U64 FILE_AB = FILE_A | (FILE_A << 1);
    const U64 FILE_GH = FILE_H | (FILE_H >> 1);
    while (original_knight_positions) {
        auto knight = original_knight_positions & - original_knight_positions;
        // get possible moves for this knight
        U64 l1 = (knight >> 1) & ~FILE_H;
        U64 l2 = (knight >> 2) & ~FILE_GH;
        U64 r1 = (knight << 1) & ~FILE_A;
        U64 r2 = (knight << 2) & ~FILE_AB;
        U64 moves = ((l1 << 16) | (l1 >> 16) | (r1 << 16) | (r1 >> 16) | (l2 <<  8) | (l2 >>  8) | (r2 <<  8) | (r2 >>  8)) & ~white_occupied;
        while (moves) {
            auto next_move = *this;
            auto knight_move = moves & -moves;
            // move knight
            next_move.set_white_knights((get_white_knights() ^ knight) | knight_move);
            // remove any black pieces in destination
            for (auto i = 6; i < 12; ++i) {
                if (next_move.bitboards[i] & knight_move) {
                    next_move.bitboards[i] ^= knight_move;
                    break;
                }
            }
            moves ^= knight_move;
            possible_moves.push_back(next_move);
        }
        original_knight_positions ^= knight;
    }
    // king because it only moves once
    U64 left = (get_white_king() >> 1) & ~FILE_H;
    U64 right = (get_white_king() << 1) & ~FILE_A;
    U64 up = (get_white_king() << 8) & ~RANK_1;
    U64 down = (get_white_king() >> 8) & ~RANK_8;
    U64 upLeft = (get_white_king() << 7) & ~FILE_H & ~RANK_1;
    U64 upRight = (get_white_king() << 9) & ~FILE_A & ~RANK_1;
    U64 downLeft = (get_white_king() >> 9) & ~FILE_H & ~RANK_8;
    U64 downRight = (get_white_king() >> 7) & ~FILE_A & ~RANK_8;
    U64 king_moves = (left | right | up | down | upLeft | upRight | downLeft | downRight) & ~white_occupied;
    while (king_moves) {
        auto next_move = *this;
        auto king_move = king_moves & -king_moves;
        // move king
        next_move.set_white_king(king_move);
        for (auto i = 6; i < 12; ++i) {
            if (next_move.bitboards[i] & king_move) {
                next_move.bitboards[i] ^= king_move;
                break;
            }
        }
        king_moves ^= king_move;
        possible_moves.push_back(next_move);
    }
    // after the next 3 i should be done with move generation (hopefully no bugs)
    // bishop
    U64 og_bishop_positions = get_white_bishops();
    while (og_bishop_positions) {
        U64 bishop = og_bishop_positions & -og_bishop_positions;
        U64 bishop_ptr = bishop;
        U64 bishop_moves = 0;
        // NW not file h and rank 1
        while ((bishop_ptr << 9) & ~FILE_H & ~RANK_1) {
            bishop_ptr <<= 9;
            if (bishop_ptr & white_occupied) break;
            bishop_moves |= bishop_moves;
            if (bishop_ptr & black_occupied) break;
        }
        bishop_ptr = bishop;
        // NE
        while ((bishop_ptr << 7) & ~FILE_A & ~RANK_1) {
            bishop_ptr <<= 7;
            if (bishop_ptr & white_occupied) break;
            bishop_moves |= bishop_moves;
            if (bishop_ptr & black_occupied) break;
        }
        bishop_ptr = bishop;
        // SW
        while ((bishop_ptr >> 7) & ~FILE_H & ~RANK_8) {
            bishop_ptr >>= 7;
            if (bishop_ptr & white_occupied) break;
            bishop_moves |= bishop_moves;
            if (bishop_ptr & black_occupied) break;
        }
        bishop_ptr = bishop;
        // SE
        while ((bishop_ptr >> 9) & ~FILE_A & ~RANK_8) {
            bishop_ptr >>= 9;
            if (bishop_ptr & white_occupied) break;
            bishop_moves |= bishop_moves;
            if (bishop_ptr & black_occupied) break;
        }
        og_bishop_positions ^= bishop;
        // do collected moves
        while (bishop_moves) {
            auto next_move = *this;
            auto bishop_move = bishop_moves & -bishop_moves;
            // move the bishop
            next_move.set_white_bishops((get_white_bishops() ^ bishop) | bishop_move);
            if (bishop_move & black_occupied) {
                for (auto i = 6; i < 12; ++i) {
                    next_move.bitboards[i] ^= bishop_move;
                    break;
                }
            }
            possible_moves.push_back(next_move);
            bishop_moves ^= bishop_move;
        }
    }
    
    
    
    // rook
    // queen

}