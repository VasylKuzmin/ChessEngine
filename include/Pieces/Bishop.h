#pragma once

#include "Piece.h"

#include <vector>

class Bishop : public Piece
{
  public:
    Bishop(Position, Color);
    std::vector<Position> getPseudoLegalMoves(Board&) const override;
};