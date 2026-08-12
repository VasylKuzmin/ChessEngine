#include <Board.h>
#include <Pieces/King.h>
#include <array>

const std::array<Offset, 8> King::movePattern = {Offset{0, 1},
                                                 Offset{1, 1},
                                                 Offset{1, 0},
                                                 Offset{1, -1},
                                                 Offset{0, -1},
                                                 Offset{-1, -1},
                                                 Offset{-1, 0},
                                                 Offset{-1, 1}};

King::King(Position position, Color color, const Board& board)
    : Piece(position, color, PieceType::King, board)
{
}

std::vector<Move> King::getPseudoLegalMoves() const
{
    auto moves = targetedMove(movePattern);

    if (board.canCastleLeft(color))
        moves.push_back(
            {position, position.offsetFromValid({-2, 0}), PieceType::King, MoveType::Castling});

    if (board.canCastleRight(color))
        moves.push_back(
            {position, position.offsetFromValid({2, 0}), PieceType::King, MoveType::Castling});

    return moves;
}

bool King::isAttacking(Position requestedTarget) const
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