#include "Position.h"

#include <stdexcept>

Position::Position(char horizontal, int vertical) : horizontal(horizontal), vertical(vertical)
{
    if (horizontal < 'a' || horizontal > 'h')
    {
        throw std::invalid_argument("Invalid horizontal position");
    }
    if (vertical < 1 || vertical > 8)
    {
        throw std::invalid_argument("Invalid vertical position");
    }
}

char Position::getHorizontal() const
{
    return horizontal;
}

int Position::getVertical() const
{
    return vertical;
}

int Position::getHorizontalIndex() const
{
    return horizontal - 'a';
}

int Position::getVerticalIndex() const
{
    return vertical - 1;
}

std::string Position::getPositionString() const
{
    return std::string(1, horizontal) + std::to_string(vertical);
}

Position Position::offset(int horizontalOffset, int verticalOffset) const
{
    return Position(horizontal + horizontalOffset, vertical + verticalOffset);
}

Position Position::fromString(const std::string& positionString)
{
    return Position(positionString[0], positionString[1] - '0');
}

bool Position::operator==(const Position & other) const
{
    return horizontal == other.horizontal && vertical == other.vertical;
}