#include "PieceFactory.h"

#include "Pieces/Pawn.h"

PieceFactory::PieceFactory()
{
    creatorFunctions[PieceType::Pawn] = [](Position position, Color color, PieceType type) {
        return std::make_unique<Pawn>(position, color, type);
    };
}

std::unique_ptr<Piece> PieceFactory::create(PieceType type, Position position, Color color) const
{
    return creatorFunctions.at(type)(position, color, type);
}