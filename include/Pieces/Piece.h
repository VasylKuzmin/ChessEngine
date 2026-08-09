#pragma once

#include "Position.h"

class Piece
{
  public:
    Piece(Position position) : position(position) {}
    void move(Position newPosition);
    void displayPosition();

    protected:
    Position position;
};