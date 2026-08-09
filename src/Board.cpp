#include "Board.h"

#include <iostream>
#include <string>
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

bool Board::isFriendly(Position position, Color color) const
{
    Piece* piece = getPiece(position);
    return piece != nullptr && piece->getColor() == color;
}

bool Board::isEnemy(Position position, Color color) const
{
    Piece* piece = getPiece(position);
    return piece != nullptr && piece->getColor() != color;
}

void Board::movePiece(Position from, Position to)
{
    int fromHorizontalIndex = from.getHorizontalIndex();
    int fromVerticalIndex = from.getVerticalIndex();
    int toHorizontalIndex = to.getHorizontalIndex();
    int toVerticalIndex = to.getVerticalIndex();
    squares[toHorizontalIndex][toVerticalIndex] =
        std::move(squares[fromHorizontalIndex][fromVerticalIndex]);
}

void Board::placePiece(PieceType type, Position position, Color color)
{
    int horizontalIndex = position.getHorizontalIndex();
    int verticalIndex = position.getVerticalIndex();
    squares[horizontalIndex][verticalIndex] = factory.create(type, position, color);
}

void Board::printPieceInfo(Position position)
{
    Piece* piece = getPiece(position);
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
    std::string input;
    std::cout << "Enter the position of the piece you want to move (e.g., e2): ";
    std::getline(std::cin, input);
    Piece* piece = getPiece(Position::fromString(input));
    std::vector<Position> possibleMoves = piece->getPseudoLegalMoves(*this);
    for (const Position& move : possibleMoves)
    {
        std::cout << "Possible move: " << move.getPositionString() << std::endl;
    }
    std::cout << "Enter the position to move to (e.g., e4): ";
    std::getline(std::cin, input);
    Position moveTo = Position::fromString(input);
    for (const Position& move : possibleMoves)
    {
        if (move == moveTo)
        {
            movePiece(piece->getPosition(), move);
            std::cout << "Moved " << piece->getColorString() << " " << piece->getTypeString() << " to "
                      << move.getPositionString() << std::endl;
            return;
        }
    }
}