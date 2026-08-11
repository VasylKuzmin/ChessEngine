#include <Board.h>
#include <Pieces/Knight.h>
#include <array>
#include <utility>

Knight::Knight(Position position, Color color) : Piece(position, color, PieceType::Knight)
{
}

std::vector<Position> Knight::getPseudoLegalMoves(Board& board) const
{
    static const std::array<Offset, 8> movePattern = {Offset{-1, 2},
                                                      Offset{1, 2},
                                                      Offset{2, 1},
                                                      Offset{2, -1},
                                                      Offset{1, -2},
                                                      Offset{-1, -2},
                                                      Offset{-2, -1},
                                                      Offset{-2, 1}};

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