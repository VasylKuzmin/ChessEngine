#pragma once

#include "Piece.h"

#include <vector>

class Knight : public Piece
{
  public:
    Knight(Position, Color, const Board&);
    std::vector<Move> getPseudoLegalMoves() const override;
    bool isAttacking(Position) const override;

  private:
    static const std::array<Offset, 8> movePattern;
};