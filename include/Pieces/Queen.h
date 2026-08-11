#pragma once

#include "Piece.h"

#include <vector>

class Queen : public Piece
{
  public:
    Queen(Position, Color);
    std::vector<Position> getPseudoLegalMoves(Board&) const override;
};