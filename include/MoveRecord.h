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
    std::unique_ptr<Piece> capturedPiece = nullptr;
    std::optional<Position> capturedPosition = std::nullopt;
};