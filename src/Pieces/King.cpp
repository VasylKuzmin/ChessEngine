#include <Board.h>
#include <Pieces/King.h>
#include <array>
#include <utility>

King::King(Position position, Color color) : Piece(position, color, PieceType::King)
{
}

std::vector<Position> King::getPseudoLegalMoves(Board& board) const
{
    static const std::array<Offset, 8> movePattern = {Offset{0, 1},
                                                      Offset{1, 1},
                                                      Offset{1, 0},
                                                      Offset{1, -1},
                                                      Offset{0, -1},
                                                      Offset{-1, -1},
                                                      Offset{-1, 0},
                                                      Offset{-1, 1}};

    std::vector<Position> moves;
    std::optional<Position> target;

    for (Offset move : movePattern)
    {
        target = position.offset(move);

        if (target && board.isEmptyOrEnemy(*target, color))
        {
            moves.push_back(*target);
        }
    }

    return moves;
}