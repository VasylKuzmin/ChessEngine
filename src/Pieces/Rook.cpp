#include <Board.h>
#include <Pieces/Rook.h>
#include <array>
#include <utility>

Rook::Rook(Position position, Color color) : Piece(position, color, PieceType::Rook)
{
}

std::vector<Position> Rook::getPseudoLegalMoves(Board& board) const
{
    static const std::array<Offset, 4> moveDirection = {
        Offset{0, 1},
        Offset{1, 0},
        Offset{0, -1},
        Offset{-1, 0},
    };

    std::vector<Position> moves;

    for (Offset direction : moveDirection)
    {
        auto target = position.offset(direction);

        while (target)
        {
            if (!board.isOccupied(*target))
            {
                moves.push_back(*target);
            }
            else if (board.isEnemy(*target, color))
            {
                moves.push_back(*target);
                break;
            }
            else
            {
                break;
            }

            target = target->offset(direction);
        }
    }

    return moves;
}