#include <Board.h>
#include <Pieces/Knight.h>
#include <array>
#include <utility>

const std::array<Offset, 8> Knight::movePattern = {Offset{-1, 2},
                                                   Offset{1, 2},
                                                   Offset{2, 1},
                                                   Offset{2, -1},
                                                   Offset{1, -2},
                                                   Offset{-1, -2},
                                                   Offset{-2, -1},
                                                   Offset{-2, 1}};

Knight::Knight(Position position, Color color, const Board& board)
    : Piece(position, color, PieceType::Knight, board)
{
}

std::vector<Move> Knight::getPseudoLegalMoves() const
{
    return targetedMove(movePattern);
}

bool Knight::isAttacking(Position requestedTarget) const
{
    for (Offset move : movePattern)
    {
        auto target = position.offset(move);
        if (target && *target == requestedTarget)
        {
            return true;
        }
    }
    return false;
}