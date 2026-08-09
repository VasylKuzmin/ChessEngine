#include "Pieces/Pawn.h"

#include "Board.h"

std::vector<Position> Pawn::getPseudoLegalMoves(Board& board) const
{
    std::vector<Position> moves;

    if ( ! board.isOccupied(position.offset(0, 1)))
    {
        moves.push_back(position.offset(0, 1));
    }

    if (board.isEnemy(position.offset(1, 1), color))
    {
        moves.push_back(position.offset(1, 1));
    }

    if (board.isEnemy(position.offset(-1, 1), color))
    {
        moves.push_back(position.offset(-1, 1));
    }

    return moves;
}