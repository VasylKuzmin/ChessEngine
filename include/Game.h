#pragma once

#include "Position.h"
#include "GameState.h"
#include "Color.h"

#include <vector>
#include <optional>
#include <array>
#include <utility>

class Board;
struct Move;

class Game
{
  public:
    std::array<std::optional<Position>, 4> getPromotionPositions() const;
    void handleClick(Board&, const std::pair<float, float>, float);
    void handlePositionClick(Board&, const std::optional<Position>);
    void handleMove(Board&, const Position);
    std::optional<Position> getSelectedPosition() const;
    std::vector<Move> getSelectedMoves() const;
    Color getCurrentPlayer() const;
    GameState getGameState() const;

  private:
    std::array<std::optional<Position>, 4> promotionPositions;
    std::optional<Position> selectedPosition = std::nullopt;
    std::vector<Move> selectedMoves;
    GameState gameState = GameState::Playing;
    Color player = Color::White;
};