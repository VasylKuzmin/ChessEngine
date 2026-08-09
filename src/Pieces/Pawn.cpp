#include "Pieces/Pawn.h"

Pawn::Pawn(Position position) : Piece(position), movePattern({{0, 1}, {1, 1}, {-1, 1}}) {}