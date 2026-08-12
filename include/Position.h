#pragma once

#include "Offset.h"

#include <optional>
#include <string>

class Position
{
  public:
    static std::optional<Position> tryCreate(char horizontal, int vertical);
    static Position fromValid(char horizontal, int vertical);
    char getHorizontal() const;
    int getVertical() const;
    int getHorizontalIndex() const; // returns 0-based index for horizontal position
    int getVerticalIndex() const;   // returns 0-based index for vertical position
    std::string getPositionString() const;
    std::optional<Position> offset(Offset offset) const;
    Position offsetFromValid(Offset offset) const;
    static std::optional<Position> fromString(const std::string& positionString);
    bool operator==(const Position& other) const;

  private:
    char horizontal;
    int vertical;
    Position(char horizontal, int vertical) : horizontal(horizontal), vertical(vertical) {};
    // a successfully constructed Position is guaranteed to be valid
};