#pragma once

#include "Piece.h"

#include <vector>

class King : public Piece
{
  public:
    King(Position, Color);
    std::vector<Position> getPseudoLegalMoves(Board&) const override;
};