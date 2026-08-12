#include <Board.h>
#include <Pieces/Queen.h>
#include <array>

const std::array<Offset, 8> Queen::moveDirections = {
    Offset{0, 1},
    Offset{1, 1},
    Offset{1, 0},
    Offset{1, -1},
    Offset{0, -1},
    Offset{-1, -1},
    Offset{-1, 0},
    Offset{-1, 1},
};

Queen::Queen(Position position, Color color, const Board& board)
    : Piece(position, color, PieceType::Queen, board)
{
}

std::vector<Move> Queen::getPseudoLegalMoves() const
{
    return multiDirectionalMove(moveDirections);
}

bool Queen::isAttacking(Position requestedTarget) const
{
    auto targets = getPseudoLegalMoves();
    for (const auto& target : targets)
    {
        if (target.to == requestedTarget)
        {
            return true;
        }
    }
    return false;
}