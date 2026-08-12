#include "Board.h"

#include "Pieces/Bishop.h"
#include "Pieces/King.h"
#include "Pieces/Knight.h"
#include "Pieces/Pawn.h"
#include "Pieces/Queen.h"
#include "Pieces/Rook.h"

#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <utility>

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

bool Board::canCastleRight(Color color) const
{
    Position position = (color == Color::White) ? whiteKingPosition : blackKingPosition;
    bool hasRight = (color == Color::White) ? castlingRights.whiteKing : castlingRights.blackKing;
    bool rookHasRight =
        (color == Color::White) ? castlingRights.whiteKingsRook : castlingRights.blackKingsRook;
    bool notInCheck = !isInCheck(color);
    Position square = position.offsetFromValid({1, 0});
    bool betweenSquare = isEmpty(square) && !isUnderAttack(square, color);

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

    Position square = position.offsetFromValid({-1, 0});
    bool betweenSquare = isEmpty(square) && !isUnderAttack(square, color);

    square = position.offsetFromValid({-2, 0});
    bool endSquare = isEmpty(square);

    return hasRight && rookHasRight && !isInCheck(color) && betweenSquare && endSquare;
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
    placePiece(lastMove.move.from, takePiece(lastMove.move.to));

    if (lastMove.capturedPiece)
    {
        placePiece(*lastMove.capturedPosition, std::move(lastMove.capturedPiece));
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

    castlingRights = lastMove.castlingRights;
    moveHistory.pop_back();
}

void Board::movePiece(Move move)
{
    MoveRecord record{move};
    record.castlingRights = castlingRights;

    if (move.type == MoveType::EnPassant)
    {
        Position enPassantTarget =
            move.to.offset({0, -getPiece(move.from)->getDirection()}).value();
        record.capturedPiece = takePiece(enPassantTarget);
        record.capturedPosition = enPassantTarget;
    }
    else if (move.type == MoveType::Capture)
    {
        record.capturedPiece = takePiece(move.to);
        record.capturedPosition = move.to;

        if (record.capturedPiece->getType() == PieceType::Rook)
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

void Board::promptUserMove()
{
    setUpCastlingRights();

    while (true)
    {
        std::string input;
        std::cout << "Enter the position of the piece you want to move: ";
        std::getline(std::cin, input);
        auto piece = getPiece(*Position::fromString(input));
        if (piece == nullptr)
        {
            continue;
        }
        auto possibleMoves = getLegalMoves(*piece);
        if (possibleMoves.empty())
        {
            continue;
        }
        for (const Move& move : possibleMoves)
        {
            std::cout << "Possible move: " << move.to.getPositionString() << std::endl;
        }
        std::cout << "Enter the position to move to: ";
        std::getline(std::cin, input);
        auto moveTo = Position::fromString(input);
        for (const Move& move : possibleMoves)
        {
            if (move.to == *moveTo)
            {
                movePiece(move);
                std::cout << "Moved " << piece->getColorString() << " " << piece->getTypeString()
                          << " to " << move.to.getPositionString() << std::endl;

                Color color =
                    (getPiece(move.to)->getColor() == Color::White) ? Color::Black : Color::White;
                if (isCheckmate(color))
                {
                    std::cout << std::endl
                              << piece->getColorString() << " checkmated their opponent"
                              << std::endl;
                    return;
                }

                if (isStalemate(color))
                {
                    std::cout << std::endl << "stalemate" << std::endl;
                    return;
                }
            }
        }
    }
}