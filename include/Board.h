#pragma once

#include "CastlingRights.h"
#include "Color.h"
#include "Move.h"
#include "MoveRecord.h"
#include "PieceType.h"
#include "Pieces/Piece.h"
#include "Position.h"

#include <array>
#include <memory>
#include <vector>

class Board
{
  public:
    Board();
    Piece* getPiece(Position) const;
    bool isOccupied(Position) const;
    bool isEmpty(Position) const;
    bool isFriendly(Position, Color) const;
    bool isEnemy(Position, Color) const;
    bool canEnPassant(Position, Position) const;
    bool isUnderAttack(Position, Color) const;
    bool isInCheck(Color) const;
    bool canCastleLeft(Color) const;
    bool canCastleRight(Color) const;
    bool isCheckmate(Color);
    bool isStalemate(Color);
    std::optional<Position> getCheckedKing() const;
    const MoveRecord* getLastMove() const;

    std::vector<Move> getLegalMoves(const Piece&);
    bool hasLegalMoves(Color);
    void movePiece(Move);
    void undoLastMove();
    std::unique_ptr<Piece> takePiece(Position);
    void placePiece(Position, std::unique_ptr<Piece>);
    void createPiece(PieceType, Position, Color);
    void setUpCastlingRights();
    void setUpDefault();

    // temporary for debugging purposes
    void printPieceInfo(Position) const;
    //void promptUserMove();
    //

  private:
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> squares;
    std::vector<MoveRecord> moveHistory;
    CastlingRights castlingRights;
    Position whiteKingPosition = Position::fromValid('e', 1);
    Position blackKingPosition = Position::fromValid('e', 8);
};