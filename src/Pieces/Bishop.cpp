#include <Board.h>
#include <Pieces/Bishop.h>
#include <array>
#include <utility>

const std::array<Offset, 4> Bishop::moveDirections = {
    Offset{1, 1},
    Offset{1, -1},
    Offset{-1, -1},
    Offset{-1, 1},
};

Bishop::Bishop(Position position, Color color, const Board& board) : Piece(position, color, PieceType::Bishop, board)
{
}

std::vector<Move> Bishop::getPseudoLegalMoves() const
{
    return multiDirectionalMove(moveDirections);
}

bool Bishop::isAttacking(Position requestedTarget) const
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