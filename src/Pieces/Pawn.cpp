
#include "Pieces/Pawn.h"

#include "Board.h"

Pawn::Pawn(Position position, Color color, const Board& board)
    : Piece(position, color, PieceType::Pawn, board)
{
}

std::vector<Move> Pawn::getPseudoLegalMoves() const
{
    std::vector<Move> moves;

    // Forward one
    auto target = position.offset({0, getDirection()});
    if (target && board.isEmpty(*target))
    {
        moves.push_back({position, *target, PieceType::Pawn});
    }

    // Diagonal captures / en passant
    for (int horizontal : {-1, 1})
    {
        auto target = position.offset({horizontal, getDirection()});

        if (!target)
            continue;

        if (board.isEnemy(*target, color))
        {
            moves.push_back({position, *target, PieceType::Pawn, MoveType::Capture});
        }
        else if (board.canEnPassant(position, *target))
        {
            moves.push_back({position, *target, PieceType::Pawn, MoveType::EnPassant});
        }
    }

    // Forward two
    auto between = position.offset({0, getDirection()});
    target = position.offset({0, 2 * getDirection()});
    bool hasMoved =
        (color == Color::White) ? position.getVertical() != 2 : position.getVertical() != 7;

    if (target && between && !hasMoved && board.isEmpty(*target) && board.isEmpty(*between))
    {
        moves.push_back({position, *target, PieceType::Pawn, MoveType::Charge});
    }

    return moves;
}

bool Pawn::isAttacking(Position target) const
{
    for (int horizontal : {-1, 1})
    {
        auto attackPosition = position.offset({horizontal, getDirection()});
        if (attackPosition && *attackPosition == target)
        {
            return true;
        }
    }
    return false;
}