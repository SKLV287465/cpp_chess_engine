#include "../engine/Board.hpp"
#include <iostream>
#include <list>
#include "../../include/constants.hpp"

void print_formatted(U64 input) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            std::cout << (((((input << (i * 8)) >> 56) << (j + 56)) >> (63))) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}

int main() {
    Board board;
    board.set_white_pawns(STARTING_POSITIONS[0]);
	board.set_white_knights(STARTING_POSITIONS[1]);
	board.set_white_bishops(STARTING_POSITIONS[2]);
	board.set_white_rooks(STARTING_POSITIONS[3]);
	board.set_white_queens(STARTING_POSITIONS[4]);
	board.set_white_king(STARTING_POSITIONS[5]);
	board.set_black_pawns(STARTING_POSITIONS[6]);
	board.set_black_knights(STARTING_POSITIONS[7]);
	board.set_black_bishops(STARTING_POSITIONS[8]);
	board.set_black_rooks(STARTING_POSITIONS[9]);
	board.set_black_queens(STARTING_POSITIONS[10]);
	board.set_black_king(STARTING_POSITIONS[11]);
    auto moves = board.generate_white_moves();
    for (auto i : moves) {
        print_formatted(i.get_white_pawns());
    }
    std::cout << moves.size() << std::endl;
}