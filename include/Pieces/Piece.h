#pragma once

#include "Color.h"
#include "PieceType.h"
#include "Position.h"

#include <string_view>
#include <vector>

class Board; // Forward declaration of Board class

class Piece
{
  public:
    Piece(Position position, Color color, PieceType type);
    Color getColor() const;
    std::string_view getColorString() const;
    PieceType getType() const;
    std::string_view getTypeString() const;
    Position getPosition() const;
    virtual std::vector<Position> getPseudoLegalMoves(Board& board) const = 0;

  protected:
    Position position;
    Color color;
    PieceType type;
};