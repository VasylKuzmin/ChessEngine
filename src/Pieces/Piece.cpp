#include "Pieces/Piece.h"
#include "Board.h"

Piece::Piece(Position position, Color color, PieceType type)
    : position(position), color(color), type(type) {}

void Piece::setPosition(Position newPosition)
{
    position = newPosition;
    hasMoved = true;
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