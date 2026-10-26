#include "Board.h"

#include "Pieces/Bishop.h"
#include "Pieces/King.h"
#include "Pieces/Knight.h"
#include "Pieces/Pawn.h"
#include "Pieces/Queen.h"
#include "Pieces/Rook.h"

#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

Board::Board()
{
}

Piece* Board::getPiece(Position position) const
{
    int horizontalIndex = position.getHorizontalIndex();
    int verticalIndex = position.getVerticalIndex();
    return squares[horizontalIndex][verticalIndex].get();
}

bool Board::isOccupied(Position position) const
{
    return getPiece(position) != nullptr;
}

bool Board::isEmpty(Position position) const
{
    return getPiece(position) == nullptr;
}

bool Board::isFriendly(Position position, Color color) const
{
    auto piece = getPiece(position);
    return piece != nullptr && piece->getColor() == color;
}

bool Board::isEnemy(Position position, Color color) const
{
    auto piece = getPiece(position);
    return piece != nullptr && piece->getColor() != color;
}

bool Board::isUnderAttack(Position position, Color color) const
{
    for (const auto& line : squares)
    {
        for (const auto& piece : line)
        {
            if (piece && piece->getColor() != color && piece->isAttacking(position))
            {
                return true;
            }
        }
    }

    return false;
}

bool Board::isInCheck(Color color) const
{
    const Position kingPosition = (color == Color::White) ? whiteKingPosition : blackKingPosition;

    return isUnderAttack(kingPosition, color);
}

bool Board::hasLegalMoves(Color color)
{
    for (const auto& line : squares)
    {
        for (const auto& piece : line)
        {
            if (piece && piece->getColor() == color && !getLegalMoves(*piece).empty())
                return true;
        }
    }

    return false;
}

bool Board::isCheckmate(Color color)
{
    return isInCheck(color) && !hasLegalMoves(color);
}

bool Board::isStalemate(Color color)
{
    return !isInCheck(color) && !hasLegalMoves(color);
}

std::optional<Position> Board::getCheckedKing() const
{
    if (isInCheck(Color::White))
        return whiteKingPosition;

    if (isInCheck(Color::Black))
        return blackKingPosition;

    return std::nullopt;
}

bool Board::canCastleRight(Color color) const
{
    Position position = (color == Color::White) ? whiteKingPosition : blackKingPosition;
    bool hasRight = (color == Color::White) ? castlingRights.whiteKing : castlingRights.blackKing;
    bool rookHasRight =
        (color == Color::White) ? castlingRights.whiteKingsRook : castlingRights.blackKingsRook;
    bool notInCheck = !isInCheck(color);
    if (!position.offset({1, 0}))
        return false;
    Position square = position.offsetFromValid({1, 0});
    bool betweenSquare = isEmpty(square) && !isUnderAttack(square, color);

    if (!position.offset({2, 0}))
        return false;
    square = position.offsetFromValid({2, 0});
    bool endSquare = isEmpty(square);

    return hasRight && rookHasRight && notInCheck && betweenSquare && endSquare;
}

bool Board::canCastleLeft(Color color) const
{
    Position position = (color == Color::White) ? whiteKingPosition : blackKingPosition;
    bool hasRight = (color == Color::White) ? castlingRights.whiteKing : castlingRights.blackKing;
    bool rookHasRight =
        (color == Color::White) ? castlingRights.whiteQueensRook : castlingRights.blackQueensRook;
    bool notInCheck = !isInCheck(color);

    if (!position.offset({-1, 0}))
        return false;
    Position square = position.offsetFromValid({-1, 0});
    bool betweenSquare = isEmpty(square) && !isUnderAttack(square, color);

    if (!position.offset({-2, 0}))
        return false;
    square = position.offsetFromValid({-2, 0});
    bool endSquare = isEmpty(square);

    return hasRight && rookHasRight && notInCheck && betweenSquare && endSquare;
}

std::vector<Move> Board::getLegalMoves(const Piece& piece)
{
    auto pseudoLegalMoves = piece.getPseudoLegalMoves();
    std::vector<Move> legalMoves;
    Color color = piece.getColor();

    for (const Move& move : pseudoLegalMoves)
    {
        movePiece(move);

        if (!isInCheck(color))
        {
            legalMoves.push_back(move);
        }

        undoLastMove();
    }

    return legalMoves;
}

void Board::undoLastMove()
{
    if (moveHistory.empty())
        return;

    MoveRecord& lastMove = moveHistory.back();
    Color color = getPiece(lastMove.move.to)->getColor();
    Color capturedColor = (color == Color::White) ? Color::Black : Color::White;
    placePiece(lastMove.move.from, takePiece(lastMove.move.to));

    if (lastMove.capturedPiece)
    {
        createPiece(*lastMove.capturedPiece, *lastMove.capturedPosition, capturedColor);
    }

    if (lastMove.move.pieceType == PieceType::King)
    {
        if (getPiece(lastMove.move.from)->getColor() == Color::White)
            whiteKingPosition = lastMove.move.from;
        else
            blackKingPosition = lastMove.move.from;
    }

    if (lastMove.move.type == MoveType::Castling)
    {
        // undo rook movement

        if (lastMove.move.to.getHorizontal() > lastMove.move.from.getHorizontal())
        {
            Position toH = lastMove.move.to.offset({1, 0}).value();
            Position fromF = lastMove.move.to.offset({-1, 0}).value();
            placePiece(toH, takePiece(fromF));
        }
        else
        {
            Position toA = lastMove.move.to.offset({-2, 0}).value();
            Position fromD = lastMove.move.to.offset({1, 0}).value();
            placePiece(toA, takePiece(fromD));
        }
    }

    if (lastMove.wasPromotion)
    {
        createPiece(PieceType::Pawn, lastMove.move.from, color);
    }

    castlingRights = lastMove.castlingRights;
    moveHistory.pop_back();
}

void Board::promoteLastMovedPiece(PieceType type, Color color)
{
    moveHistory.back().wasPromotion = true;
    createPiece(type, moveHistory.back().move.to, color);
}

void Board::movePiece(Move move)
{
    MoveRecord record{move};
    record.castlingRights = castlingRights;

    if (move.type == MoveType::EnPassant)
    {
        Position enPassantTarget =
            move.to.offset({0, -getPiece(move.from)->getDirection()}).value();
        record.capturedPiece = PieceType::Pawn;
        record.capturedPosition = enPassantTarget;
        removePiece(enPassantTarget);
    }
    else if (move.type == MoveType::Capture)
    {
        record.capturedPiece = getPiece(move.to)->getType();
        record.capturedPosition = move.to;

        if (record.capturedPiece == PieceType::Rook)
        {
            if (*record.capturedPosition == Position::fromValid('a', 1))
                castlingRights.whiteQueensRook = false;
            else if (*record.capturedPosition == Position::fromValid('h', 1))
                castlingRights.whiteKingsRook = false;
            else if (*record.capturedPosition == Position::fromValid('a', 8))
                castlingRights.blackQueensRook = false;
            else if (*record.capturedPosition == Position::fromValid('h', 8))
                castlingRights.blackKingsRook = false;
        }
    }
    else if (move.type == MoveType::Castling)
    {
        if (move.to.getHorizontal() > move.from.getHorizontal())
        {
            placePiece(move.to.offsetFromValid({-1, 0}),
                       takePiece(move.to.offsetFromValid({1, 0})));
        }
        else
        {
            placePiece(move.to.offsetFromValid({1, 0}),
                       takePiece(move.to.offsetFromValid({-2, 0})));
        }
    }

    placePiece(move.to, takePiece(move.from));

    Piece* piece = getPiece(move.to);
    if (piece->getType() == PieceType::King)
    {
        if (piece->getColor() == Color::White)
        {
            whiteKingPosition = move.to;
            castlingRights.whiteKing = false;
        }
        else
        {
            blackKingPosition = move.to;
            castlingRights.blackKing = false;
        }
    }

    if (piece->getType() == PieceType::Rook)
    {
        if (piece->getColor() == Color::White)
        {
            if (move.from == Position::fromValid('a', 1))
                castlingRights.whiteQueensRook = false;
            else if (move.from == Position::fromValid('h', 1))
                castlingRights.whiteKingsRook = false;
        }
        else
        {
            if (move.from == Position::fromValid('a', 8))
                castlingRights.blackQueensRook = false;
            else if (move.from == Position::fromValid('h', 8))
                castlingRights.blackKingsRook = false;
        }
    }

    record.movedPiece = piece->getType();
    record.movedPieceColor = piece->getColor();
    moveHistory.push_back(std::move(record));
}

const MoveRecord* Board::getLastMove() const
{
    if (moveHistory.empty())
    {
        return nullptr;
    }
    return &moveHistory.back();
}

std::unique_ptr<Piece> Board::takePiece(Position position)
{
    return std::move(squares[position.getHorizontalIndex()][position.getVerticalIndex()]);
}

void Board::removePiece(const Position position)
{
    squares[position.getHorizontalIndex()][position.getVerticalIndex()].reset();
}

void Board::placePiece(Position position, std::unique_ptr<Piece> piece)
{
    piece->setPosition(position);
    squares[position.getHorizontalIndex()][position.getVerticalIndex()] = std::move(piece);
}

void Board::createPiece(PieceType type, Position position, Color color)
{
    using PieceCreator = std::function<std::unique_ptr<Piece>(Position, Color, const Board&)>;

    static const std::unordered_map<PieceType, PieceCreator> creatorFunctions = {
        {PieceType::Pawn,
         [](Position position, Color color, const Board& board) {
             return std::make_unique<Pawn>(position, color, board);
         }},
        {PieceType::Knight,
         [](Position position, Color color, const Board& board) {
             return std::make_unique<Knight>(position, color, board);
         }},
        {PieceType::King,
         [](Position position, Color color, const Board& board) {
             return std::make_unique<King>(position, color, board);
         }},
        {PieceType::Rook,
         [](Position position, Color color, const Board& board) {
             return std::make_unique<Rook>(position, color, board);
         }},
        {PieceType::Bishop,
         [](Position position, Color color, const Board& board) {
             return std::make_unique<Bishop>(position, color, board);
         }},
        {PieceType::Queen,
         [](Position position, Color color, const Board& board) {
             return std::make_unique<Queen>(position, color, board);
         }}

    };

    placePiece(position, creatorFunctions.at(type)(position, color, *this));
}

void Board::printPieceInfo(Position position) const
{
    auto piece = getPiece(position);
    if (piece)
    {
        std::cout << piece->getTypeString() << " at " << position.getPositionString() << " colored "
                  << piece->getColorString() << std::endl;
    }
    else
    {
        std::cout << "No piece at this position." << std::endl;
    }
}

bool Board::canEnPassant(Position from, Position to) const
{
    if (moveHistory.empty())
    {
        return false;
    }
    return getLastMove()->move.type == MoveType::Charge &&
           getLastMove()->move.to == to.offset({0, -(getPiece(from)->getDirection())}).value();
}

void Board::setUpCastlingRights()
{
    Piece* target = getPiece(Position::fromValid('e', 1));
    if (target && target->getType() == PieceType::King && target->getColor() == Color::White)
        castlingRights.whiteKing = true;
    target = getPiece(Position::fromValid('a', 1));
    if (target && target->getType() == PieceType::Rook && target->getColor() == Color::White)
        castlingRights.whiteQueensRook = true;
    target = getPiece(Position::fromValid('h', 1));
    if (target && target->getType() == PieceType::Rook && target->getColor() == Color::White)
        castlingRights.whiteKingsRook = true;
    target = getPiece(Position::fromValid('e', 8));
    if (target && target->getType() == PieceType::King && target->getColor() == Color::Black)
        castlingRights.blackKing = true;
    target = getPiece(Position::fromValid('a', 8));
    if (target && target->getType() == PieceType::Rook && target->getColor() == Color::Black)
        castlingRights.blackQueensRook = true;
    target = getPiece(Position::fromValid('h', 8));
    if (target && target->getType() == PieceType::Rook && target->getColor() == Color::Black)
        castlingRights.blackKingsRook = true;
}

void Board::setUpDefault()
{
    createPiece(PieceType::Pawn, Position::fromValid('a', 2), Color::White);
    createPiece(PieceType::Pawn, Position::fromValid('b', 2), Color::White);
    createPiece(PieceType::Pawn, Position::fromValid('c', 2), Color::White);
    createPiece(PieceType::Pawn, Position::fromValid('d', 2), Color::White);
    createPiece(PieceType::Pawn, Position::fromValid('e', 2), Color::White);
    createPiece(PieceType::Pawn, Position::fromValid('f', 2), Color::White);
    createPiece(PieceType::Pawn, Position::fromValid('g', 2), Color::White);
    createPiece(PieceType::Pawn, Position::fromValid('h', 2), Color::White);
    createPiece(PieceType::Rook, Position::fromValid('a', 1), Color::White);
    createPiece(PieceType::Rook, Position::fromValid('h', 1), Color::White);
    createPiece(PieceType::Knight, Position::fromValid('b', 1), Color::White);
    createPiece(PieceType::Knight, Position::fromValid('g', 1), Color::White);
    createPiece(PieceType::Bishop, Position::fromValid('c', 1), Color::White);
    createPiece(PieceType::Bishop, Position::fromValid('f', 1), Color::White);
    createPiece(PieceType::Queen, Position::fromValid('d', 1), Color::White);
    createPiece(PieceType::King, Position::fromValid('e', 1), Color::White);
    createPiece(PieceType::Pawn, Position::fromValid('a', 7), Color::Black);
    createPiece(PieceType::Pawn, Position::fromValid('b', 7), Color::Black);
    createPiece(PieceType::Pawn, Position::fromValid('c', 7), Color::Black);
    createPiece(PieceType::Pawn, Position::fromValid('d', 7), Color::Black);
    createPiece(PieceType::Pawn, Position::fromValid('e', 7), Color::Black);
    createPiece(PieceType::Pawn, Position::fromValid('f', 7), Color::Black);
    createPiece(PieceType::Pawn, Position::fromValid('g', 7), Color::Black);
    createPiece(PieceType::Pawn, Position::fromValid('h', 7), Color::Black);
    createPiece(PieceType::Rook, Position::fromValid('a', 8), Color::Black);
    createPiece(PieceType::Rook, Position::fromValid('h', 8), Color::Black);
    createPiece(PieceType::Knight, Position::fromValid('b', 8), Color::Black);
    createPiece(PieceType::Knight, Position::fromValid('g', 8), Color::Black);
    createPiece(PieceType::Bishop, Position::fromValid('c', 8), Color::Black);
    createPiece(PieceType::Bishop, Position::fromValid('f', 8), Color::Black);
    createPiece(PieceType::Queen, Position::fromValid('d', 8), Color::Black);
    createPiece(PieceType::King, Position::fromValid('e', 8), Color::Black);

    setUpCastlingRights();
}

std::vector<MoveRecord> Board::getMoveHistory() const
{
    return moveHistory;
}