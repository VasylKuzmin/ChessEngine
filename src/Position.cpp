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