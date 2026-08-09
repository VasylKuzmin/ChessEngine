#pragma once

#include <array>
#include <string_view>

enum class PieceType
{
    Pawn,
    Rook,
    Knight,
    Bishop,
    Queen,
    King
};

constexpr std::array<std::string_view, 6> pieceTypeNames = 
{
    "Pawn",
    "Rook",
    "Knight",
    "Bishop",
    "Queen",
    "King"
}; // permanent array of strings, indexed by PieceType enum values (the order must always match the enum)