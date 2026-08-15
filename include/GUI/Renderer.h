#pragma once

#include <utility>
#include <optional>
#include <vector>
#include <SDL3/SDL.h>

class Game;
class Position;
class Board;
class Piece;
struct Move;

class Renderer
{
  public:
    explicit Renderer(Game&);

    bool initialize();
    void shutdown();

    void renderMoveIndicator(Position);
    void renderPiece(const Piece&);
    void renderSquareBorder(Position, SDL_Color, float inset);
    void render(const Board&);

    static std::pair<float, float> convertCoords(std::pair<char, int>);
    static std::pair<char, int> convertCoords(std::pair<int, int>);
    static std::pair<char, int> convertCoords(std::pair<float, float>);

  private:
    static constexpr float boardSize = 800.0f;
    static constexpr int numberOfSquares = 8;
    static constexpr float squareSize = boardSize / numberOfSquares;
    SDL_Window* window = nullptr;
    SDL_Renderer* sdlRenderer = nullptr;
    const Game& game;
};