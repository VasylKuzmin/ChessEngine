#pragma once

#include "Color.h"
#include "PieceType.h"
#include "Pieces/Piece.h"
#include "Position.h"

#include <array>
#include <memory>

class Board
{
  public:
    Board();
    Piece* getPiece(Position) const;
    bool isOccupied(Position) const;
    bool isEmpty(Position) const;
    bool isFriendly(Position, Color) const;
    bool isEnemy(Position, Color) const;
    bool isEmptyOrEnemy(Position, Color) const;
    void movePiece(Position from, Position to);
    void createPiece(PieceType, Position, Color);
    void printPieceInfo(Position) const;
    void promptUserMove();

  private:
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> squares;
};