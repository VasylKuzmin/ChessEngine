#pragma once

#include "Piece.h"

#include <vector>

class Knight : public Piece
{
  public:
    Knight(Position, Color);
    std::vector<Position> getPseudoLegalMoves(Board&) const override;
};