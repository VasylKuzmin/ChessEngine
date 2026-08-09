#pragma once

#include "Color.h"
#include "PieceType.h"
#include "Pieces/Piece.h"
#include "Position.h"

#include <functional>
#include <memory>
#include <unordered_map>

class PieceFactory
{
  public:
    PieceFactory();
    std::unique_ptr<Piece> create(PieceType type, Position position, Color color) const;

  private:
    using PieceCreator = std::function<std::unique_ptr<Piece>(Position, Color, PieceType)>;
    std::unordered_map<PieceType, PieceCreator> creatorFunctions;
};