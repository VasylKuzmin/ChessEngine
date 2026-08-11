#include "Position.h"

#include <stdexcept>

std::optional<Position> Position::tryCreate(char horizontal, int vertical)
{
    if (horizontal < 'a' || horizontal > 'h' || vertical < 1 || vertical > 8)
    {
        return std::nullopt;
    }
    
    return Position(horizontal, vertical);
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

std::optional<Position> Position::offset(Offset offset) const
{
    return Position::tryCreate(horizontal + offset.horizontal, vertical + offset.vertical);
}

std::optional<Position> Position::fromString(const std::string& positionString)
{
    return Position::tryCreate(positionString[0], positionString[1] - '0');
}

bool Position::operator==(const Position & other) const
{
    return horizontal == other.horizontal && vertical == other.vertical;
}