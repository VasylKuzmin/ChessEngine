#pragma once

#include "Color.h"
#include "PieceFactory.h"
#include "PieceType.h"
#include "Pieces/Piece.h"
#include "Position.h"

#include <array>
#include <memory>

class Board
{
  public:
    Board();
    Piece* getPiece(Position position) const;
    bool isOccupied(Position position) const;
    bool isFriendly(Position position, Color color) const;
    bool isEnemy(Position position, Color color) const;
    void movePiece(Position from, Position to);
    void placePiece(PieceType type, Position position, Color color);
    void printPieceInfo(Position position);
    void promptUserMove();

  private:
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> squares;
    PieceFactory factory;
};