#pragma once

#include "Piece.h"

#include <vector>

class Rook : public Piece
{
  public:
    Rook(Position, Color);
    std::vector<Position> getPseudoLegalMoves(Board&) const override;
};