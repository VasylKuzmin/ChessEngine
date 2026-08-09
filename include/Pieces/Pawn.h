#pragma once

#include "Piece.h"

#include <string>
#include <vector>

class Pawn : public Piece
{
  public:
    using Piece::Piece;
    std::vector<Position> getPseudoLegalMoves(Board& board) const override;
};