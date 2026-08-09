#pragma once

#include <string>

class Position
{
  public:
    Position(char horizontal,
             int vertical); // a successfully constructed Position is guaranteed to be valid

    char getHorizontal() const;
    int getVertical() const;
    int getHorizontalIndex() const; // returns 0-based index for horizontal position
    int getVerticalIndex() const;   // returns 0-based index for vertical position
    std::string getPositionString() const;
    Position offset(int horizontalOffset, int verticalOffset) const;
    static Position fromString(const std::string& positionString);
    bool operator==(const Position& other) const;

  private:
    char horizontal;
    int vertical;
};