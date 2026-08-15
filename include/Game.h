#pragma once

#include "Position.h"
#include "GameState.h"
#include "Color.h"

#include <vector>
#include <optional>

class Board;
struct Move;

class Game
{
  public:
    void handleClick(Board&, const Position);
    void handleMove(Board&, const Position);
    std::optional<Position> getSelectedPosition() const;
    std::vector<Move> getSelectedMoves() const;

  private:
    std::optional<Position> selectedPosition = std::nullopt;
    std::vector<Move> selectedMoves;
    GameState gameState = GameState::Playing;
    Color player = Color::White;
};