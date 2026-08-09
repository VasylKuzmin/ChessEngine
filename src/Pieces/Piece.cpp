#include "Pieces/Piece.h"

#include <iostream>

Piece::Piece(Position position, Color color, PieceType type)
    : position(position), color(color), type(type) {}

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