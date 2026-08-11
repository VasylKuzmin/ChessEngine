#include "DoubleDecker.h"

int main()
{
    Board board;
    board.createPiece(PieceType::Pawn, *Position::tryCreate('a', 2), Color::White);
    board.createPiece(PieceType::Pawn, *Position::tryCreate('b', 2), Color::White);
    board.createPiece(PieceType::Pawn, *Position::tryCreate('c', 2), Color::White);
    board.createPiece(PieceType::Pawn, *Position::tryCreate('d', 2), Color::White);
    board.createPiece(PieceType::Pawn, *Position::tryCreate('e', 2), Color::White);
    board.createPiece(PieceType::Pawn, *Position::tryCreate('f', 2), Color::White);
    board.createPiece(PieceType::Pawn, *Position::tryCreate('g', 2), Color::White);
    board.createPiece(PieceType::Pawn, *Position::tryCreate('h', 2), Color::White);
    board.createPiece(PieceType::Rook, *Position::tryCreate('a', 1), Color::White);
    board.createPiece(PieceType::Rook, *Position::tryCreate('h', 1), Color::White);
    board.createPiece(PieceType::Knight, *Position::tryCreate('b', 1), Color::White);
    board.createPiece(PieceType::Knight, *Position::tryCreate('g', 1), Color::White);
    board.createPiece(PieceType::Bishop, *Position::tryCreate('c', 1), Color::White);
    board.createPiece(PieceType::Bishop, *Position::tryCreate('f', 1), Color::White);
    board.createPiece(PieceType::Queen, *Position::tryCreate('d', 1), Color::White);
    board.createPiece(PieceType::King, *Position::tryCreate('e', 1), Color::White);
    board.createPiece(PieceType::Pawn, *Position::tryCreate('a', 7), Color::Black);
    board.createPiece(PieceType::Pawn, *Position::tryCreate('b', 7), Color::Black);
    board.createPiece(PieceType::Pawn, *Position::tryCreate('c', 7), Color::Black);
    board.createPiece(PieceType::Pawn, *Position::tryCreate('d', 7), Color::Black);
    board.createPiece(PieceType::Pawn, *Position::tryCreate('e', 7), Color::Black);
    board.createPiece(PieceType::Pawn, *Position::tryCreate('f', 7), Color::Black);
    board.createPiece(PieceType::Pawn, *Position::tryCreate('g', 7), Color::Black);
    board.createPiece(PieceType::Pawn, *Position::tryCreate('h', 7), Color::Black);
    board.createPiece(PieceType::Rook, *Position::tryCreate('a', 8), Color::Black);
    board.createPiece(PieceType::Rook, *Position::tryCreate('h', 8), Color::Black);
    board.createPiece(PieceType::Knight, *Position::tryCreate('b', 8), Color::Black);
    board.createPiece(PieceType::Knight, *Position::tryCreate('g', 8), Color::Black);
    board.createPiece(PieceType::Bishop, *Position::tryCreate('c', 8), Color::Black);
    board.createPiece(PieceType::Bishop, *Position::tryCreate('f', 8), Color::Black);
    board.createPiece(PieceType::Queen, *Position::tryCreate('d', 8), Color::Black);
    board.createPiece(PieceType::King, *Position::tryCreate('e', 8), Color::Black);
    board.promptUserMove();
    return 0;
}
