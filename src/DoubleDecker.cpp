#include "DoubleDecker.h"

int main()
{
    Board board;
     board.createPiece(PieceType::Pawn, Position::fromValid('a', 2), Color::White);
     board.createPiece(PieceType::Pawn, Position::fromValid('b', 2), Color::White);
     board.createPiece(PieceType::Pawn, Position::fromValid('c', 2), Color::White);
     board.createPiece(PieceType::Pawn, Position::fromValid('d', 2), Color::White);
     board.createPiece(PieceType::Pawn, Position::fromValid('e', 2), Color::White);
     board.createPiece(PieceType::Pawn, Position::fromValid('f', 2), Color::White);
     board.createPiece(PieceType::Pawn, Position::fromValid('g', 2), Color::White);
     board.createPiece(PieceType::Pawn, Position::fromValid('h', 2), Color::White);
     board.createPiece(PieceType::Rook, Position::fromValid('a', 1), Color::White);
     board.createPiece(PieceType::Rook, Position::fromValid('h', 1), Color::White);
     board.createPiece(PieceType::Knight, Position::fromValid('b', 1), Color::White);
     board.createPiece(PieceType::Knight, Position::fromValid('g', 1), Color::White);
     board.createPiece(PieceType::Bishop, Position::fromValid('c', 1), Color::White);
     board.createPiece(PieceType::Bishop, Position::fromValid('f', 1), Color::White);
     board.createPiece(PieceType::Queen, Position::fromValid('d', 1), Color::White);
     board.createPiece(PieceType::King, Position::fromValid('e', 1), Color::White);
     board.createPiece(PieceType::Pawn, Position::fromValid('a', 7), Color::Black);
     board.createPiece(PieceType::Pawn, Position::fromValid('b', 7), Color::Black);
     board.createPiece(PieceType::Pawn, Position::fromValid('c', 7), Color::Black);
     board.createPiece(PieceType::Pawn, Position::fromValid('d', 7), Color::Black);
     board.createPiece(PieceType::Pawn, Position::fromValid('e', 7), Color::Black);
     board.createPiece(PieceType::Pawn, Position::fromValid('f', 7), Color::Black);
     board.createPiece(PieceType::Pawn, Position::fromValid('g', 7), Color::Black);
     board.createPiece(PieceType::Pawn, Position::fromValid('h', 7), Color::Black);
     board.createPiece(PieceType::Rook, Position::fromValid('a', 8), Color::Black);
     board.createPiece(PieceType::Rook, Position::fromValid('h', 8), Color::Black);
     board.createPiece(PieceType::Knight, Position::fromValid('b', 8), Color::Black);
     board.createPiece(PieceType::Knight, Position::fromValid('g', 8), Color::Black);
     board.createPiece(PieceType::Bishop, Position::fromValid('c', 8), Color::Black);
     board.createPiece(PieceType::Bishop, Position::fromValid('f', 8), Color::Black);
     board.createPiece(PieceType::Queen, Position::fromValid('d', 8), Color::Black);
     board.createPiece(PieceType::King, Position::fromValid('e', 8), Color::Black);

    //board.createPiece(PieceType::King, Position::fromValid('a', 1), Color::White);
    //board.createPiece(PieceType::Queen, Position::fromValid('c', 3), Color::Black);

    board.promptUserMove();
    return 0;
}
