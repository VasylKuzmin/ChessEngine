#pragma once

class Position
{
  public:
    Position(char horizontal, int vertical);

    char getHorizontal() const;
    int getVertical() const;

    private:
    char horizontal;
    int vertical;
};