#include <Board.h>
#include <Pieces/Rook.h>
#include <array>
#include <utility>

const std::array<Offset, 4> Rook::moveDirections = {
    Offset{0, 1},
    Offset{1, 0},
    Offset{0, -1},
    Offset{-1, 0},
};

Rook::Rook(Position position, Color color, const Board& board) : Piece(position, color, PieceType::Rook, board)
{
}

std::vector<Move> Rook::getPseudoLegalMoves() const
{
    return multiDirectionalMove(moveDirections);
}

bool Rook::isAttacking(Position requestedTarget) const
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