#include <Board.h>
#include <Pieces/Queen.h>
#include <array>
#include <utility>

Queen::Queen(Position position, Color color) : Piece(position, color, PieceType::Queen)
{
}

std::vector<Position> Queen::getPseudoLegalMoves(Board& board) const
{
    static const std::array<Offset, 8> moveDirection = {
        Offset{0, 1},
        Offset{1, 1},
        Offset{1, 0},
        Offset{1, -1},
        Offset{0, -1},
        Offset{-1, -1},
        Offset{-1, 0},
        Offset{-1, 1},
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