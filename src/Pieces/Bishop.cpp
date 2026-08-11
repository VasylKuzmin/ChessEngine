#include <Board.h>
#include <Pieces/Bishop.h>
#include <array>
#include <utility>

Bishop::Bishop(Position position, Color color) : Piece(position, color, PieceType::Bishop)
{
}

std::vector<Position> Bishop::getPseudoLegalMoves(Board& board) const
{
    static const std::array<Offset, 4> moveDirection = {
        Offset{1, 1},
        Offset{1, -1},
        Offset{-1, -1},
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