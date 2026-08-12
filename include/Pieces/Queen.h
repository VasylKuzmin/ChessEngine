#pragma once

#include "Piece.h"

#include <array>
#include <vector>

class Queen : public Piece
{
  public:
    Queen(Position, Color, const Board&);
    std::vector<Move> getPseudoLegalMoves() const override;
    bool isAttacking(Position) const override;

  private:
    static const std::array<Offset, 8> moveDirections;
};