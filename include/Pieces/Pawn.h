#pragma once

#include "Piece.h"

#include <vector>

class Pawn : public Piece
{
  public:
    Pawn(Position, Color);
    std::vector<Position> getPseudoLegalMoves(Board&) const override;        
};