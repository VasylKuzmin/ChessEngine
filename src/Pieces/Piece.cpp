#include "Pieces/Piece.h"
#include <iostream>

void Piece::move(Position newPosition)
{
    position = newPosition;
}

void Piece::displayPosition()
{
    std::cout << "Position: " << position.getHorizontal() << position.getVertical() << std::endl;
}