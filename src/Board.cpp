#include "Board.h"

#include "Pieces/King.h"
#include "Pieces/Knight.h"
#include "Pieces/Pawn.h"
#include "Pieces/Rook.h"
#include "Pieces/Bishop.h"
#include "Pieces/Queen.h"

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
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

bool Board::isEmptyOrEnemy(Position position, Color color) const
{
    return isEnemy(position, color) || !isOccupied(position);
}

void Board::movePiece(Position from, Position to)
{
    squares[to.getHorizontalIndex()][to.getVerticalIndex()] =
        std::move(squares[from.getHorizontalIndex()][from.getVerticalIndex()]);
    getPiece(to)->setPosition(to);
}

void Board::createPiece(PieceType type, Position position, Color color)
{
    using PieceCreator = std::function<std::unique_ptr<Piece>(Position, Color)>;

    static const std::unordered_map<PieceType, PieceCreator> creatorFunctions = {
        {PieceType::Pawn,
         [](Position position, Color color) { return std::make_unique<Pawn>(position, color); }},
        {PieceType::Knight,
         [](Position position, Color color) { return std::make_unique<Knight>(position, color); }},
        {PieceType::King,
         [](Position position, Color color) { return std::make_unique<King>(position, color); }},
        {PieceType::Rook,
         [](Position position, Color color) { return std::make_unique<Rook>(position, color); }}, {
            PieceType::Bishop,
         [](Position position, Color color) { return std::make_unique<Bishop>(position, color); }},
        {PieceType::Queen,
         [](Position position, Color color) { return std::make_unique<Queen>(position, color); }}

    };

    squares[position.getHorizontalIndex()][position.getVerticalIndex()] =
        creatorFunctions.at(type)(position, color);
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

void Board::promptUserMove()
{
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
        auto possibleMoves = piece->getPseudoLegalMoves(*this);
        if (possibleMoves.empty())
        {
            continue;
        }
        for (const Position& move : possibleMoves)
        {
            std::cout << "Possible move: " << move.getPositionString() << std::endl;
        }
        std::cout << "Enter the position to move to: ";
        std::getline(std::cin, input);
        auto moveTo = Position::fromString(input);
        for (const Position& move : possibleMoves)
        {
            if (move == *moveTo)
            {
                movePiece(piece->getPosition(), move);
                std::cout << "Moved " << piece->getColorString() << " " << piece->getTypeString()
                          << " to " << move.getPositionString() << std::endl;
                // return;
            }
        }
    }
}