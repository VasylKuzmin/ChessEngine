#include "DoubleDecker.h"

int main()
{
    Board board;
    board.placePiece(PieceType::Pawn, Position('e', 4), Color::White);
    board.placePiece(PieceType::Pawn, Position('d', 5), Color::Black);
    board.printPieceInfo(Position('e', 4));
    board.printPieceInfo(Position('d', 5));
    board.promptUserMove();
    return 0;
}
