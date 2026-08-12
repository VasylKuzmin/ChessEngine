#pragma once

#include "Piece.h"

#include <vector>

class Rook : public Piece
{
  public:
    Rook(Position, Color, const Board&);
    std::vector<Move> getPseudoLegalMoves() const override;
    bool isAttacking(Position) const override;

  private:
    static const std::array<Offset, 4> moveDirections;
};