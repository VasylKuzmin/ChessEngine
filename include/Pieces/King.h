#pragma once

#include "Piece.h"

#include <vector>

class King : public Piece
{
  public:
    King(Position, Color, const Board&);
    std::vector<Move> getPseudoLegalMoves() const override;
    bool isAttacking(Position) const override;

  private:
    static const std::array<Offset, 8> movePattern;
};