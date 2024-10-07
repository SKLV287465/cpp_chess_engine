#ifndef Board_HPP_
#define Board_HPP_

#include <array>
#include <constants.hpp>

class GameBoard {
    public:
    // constants
    /**
     * creates a new board with starting positions
     */
    GameBoard() {
        
    }

    


    private:
    // true for white and false for black
    bool turn;
    bool play_as;
    
};

#endif /* Board_HPP_ */