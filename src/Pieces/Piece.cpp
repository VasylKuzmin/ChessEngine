#include "Pieces/Piece.h"

#include "Board.h"

Piece::Piece(Position position, Color color, PieceType type, const Board& board)
    : position(position), color(color), type(type), board(board)
{
}

void Piece::setPosition(Position newPosition)
{
    position = newPosition;
}

Color Piece::getColor() const
{
    return color;
}

std::string_view Piece::getColorString() const
{
    return colorNames[static_cast<int>(color)];
}

PieceType Piece::getType() const
{
    return type;
}

std::string_view Piece::getTypeString() const
{
    return pieceTypeNames[static_cast<int>(type)];
}

Position Piece::getPosition() const
{
    return position;
}

int Piece::getDirection() const
{
    return (color == Color::White) ? 1 : -1;
}

std::vector<Move> Piece::multiDirectionalMove(std::span<const Offset> moveDirections) const
{
    std::vector<Move> moves;

    for (Offset direction : moveDirections)
    {
        auto target = position.offset(direction);

        while (target)
        {
            if (board.isEmpty(*target))
            {
                moves.push_back({position, *target, type});
            }
            else if (board.isEnemy(*target, color))
            {
                moves.push_back({position, *target, type, MoveType::Capture});
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

std::vector<Move> Piece::targetedMove(std::span<const Offset> moveTargets) const
{
    std::vector<Move> moves;

    for (Offset move : moveTargets)
    {
        auto target = position.offset(move);

        if (!target)
            continue;

        if (board.isEmpty(*target))
        {
            moves.push_back({position, *target, type});
        }
        else if (board.isEnemy(*target, color))
        {
            moves.push_back({position, *target, type, MoveType::Capture});
        }
    }

    return moves;
}