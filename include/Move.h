#pragma once

#include "Position.h"
#include "PieceType.h"

enum class MoveType
{
    Normal,
    Capture,
    Castling,
    EnPassant,
    Promotion,
    Charge
};

struct Move
{
    Position from;
    Position to;
    PieceType pieceType;
    MoveType type = MoveType::Normal;
};