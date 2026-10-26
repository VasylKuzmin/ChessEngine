#pragma once

#include "CastlingRights.h"
#include "Move.h"
#include "Pieces/Piece.h"
#include "Position.h"

#include <memory>
#include <optional>

struct MoveRecord
{
    Move move;
    CastlingRights castlingRights;
    std::optional<PieceType> capturedPiece = std::nullopt;
    std::optional<Position> capturedPosition = std::nullopt;
    PieceType movedPiece;
    Color movedPieceColor;
    bool wasPromotion = false;
};