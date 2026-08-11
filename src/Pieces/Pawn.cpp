#include "Pieces/Pawn.h"

#include "Board.h"

Pawn::Pawn(Position position, Color color) : Piece(position, color, PieceType::Pawn)
{
}

std::vector<Position> Pawn::getPseudoLegalMoves(Board& board) const 
{
    std::vector<Position> moves;
    
    auto target = position.offset({0, getDirection()});
    if (target && board.isEmpty(*target))
    {
        moves.push_back(*target);
    }

    target = position.offset({1, getDirection()});
    if (target && board.isEnemy(*target, color))
    {
        moves.push_back(*target);
    }

    target = position.offset({-1, getDirection()});
    if (target && board.isEnemy(*target, color))
    {
        moves.push_back(*target);
    }

    target = position.offset({0, 2 * getDirection()});
    if (target && !hasMoved && board.isEmpty(*target) &&
        board.isEmpty(*position.offset({0, getDirection()})))
    {
        moves.push_back(*target);
    }

    return moves;
}