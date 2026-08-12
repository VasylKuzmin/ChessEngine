#pragma once

#include "Piece.h"

#include <vector>

class Pawn : public Piece
{
  public:
    Pawn(Position, Color, const Board&);
    std::vector<Move> getPseudoLegalMoves() const override;
    bool isAttacking(Position) const override;
};