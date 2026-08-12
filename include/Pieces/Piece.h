#pragma once

#include "Color.h"
#include "Move.h"
#include "PieceType.h"
#include "Position.h"

#include <span>
#include <string_view>
#include <vector>

class Board; // Forward declaration of Board class

class Piece
{
  public:
    Piece(Position, Color, PieceType, const Board&);
    void setPosition(Position);
    Color getColor() const;
    virtual int getDirection() const; // returns 1 for White and -1 for Black
    std::string_view getColorString() const;
    PieceType getType() const;
    std::string_view getTypeString() const;
    Position getPosition() const;
    std::vector<Move> multiDirectionalMove(std::span<const Offset>) const;
    std::vector<Move> targetedMove(std::span<const Offset>) const;
    virtual bool isAttacking(Position) const = 0;
    virtual std::vector<Move> getPseudoLegalMoves() const = 0;

  protected:
    Position position;
    Color color;
    PieceType type;
    const Board& board;
};