#pragma once

#include "GUI/SDLContext.h"
#include "GUI/TextureHandler.h"
#include "Position.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <array>
#include <memory>
#include <utility>
#include <optional>

class Game;
class Board;
class Piece;
struct Move;

class Renderer
{
  public:
    explicit Renderer(Game&);
    ~Renderer();

    void initialize();

    void renderMoveIndicator(Position);
    void renderPromotionChoices(Color);
    void renderPiece(const Piece&);
    void renderSquareBorder(Position, SDL_Color, float inset);
    void render(const Board&);
    float getSquareSize() const;

    static std::pair<float, float> convertCoords(std::pair<char, int>);
    static std::pair<char, int> convertCoords(std::pair<int, int>);
    static std::pair<char, int> convertCoords(std::pair<float, float>);

  private:
    static constexpr float boardSize = 1024.0f;
    static constexpr int numberOfSquares = 8;
    static constexpr float squareSize = boardSize / numberOfSquares;
    SDLContext sdlContext;
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window{nullptr, SDL_DestroyWindow};
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> sdlRenderer{nullptr,
                                                                              SDL_DestroyRenderer};
    std::unique_ptr<TextureHandler> textureHandler;

    const Game& game;
    std::array<std::optional<Position>, 4> promotionPositions;
};