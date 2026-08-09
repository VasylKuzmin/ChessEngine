#pragma once

#include "Piece.h" 
#include "Offset.h"
#include <vector>

class Pawn : public Piece
{
  public:
    Pawn(Position position);

  private:
    std::vector<Offset> movePattern;
};