#include "GUI/Renderer.h"

#include "Board.h"
#include "Color.h"
#include "Game.h"
#include "Pieces/Piece.h"
#include "Position.h"

Renderer::Renderer(Game& game) : game(game)
{
}

std::pair<char, int> Renderer::convertCoords(std::pair<int, int> screenCoords)
{
    return convertCoords(
        std::pair{static_cast<float>(screenCoords.first), static_cast<float>(screenCoords.second)});
}

std::pair<float, float> Renderer::convertCoords(std::pair<char, int> chessCoords)
{
    return std::pair{(chessCoords.first - 'a') * squareSize,
                     (numberOfSquares - chessCoords.second) * squareSize};
}

std::pair<char, int> Renderer::convertCoords(std::pair<float, float> screenCoords)
{
    return std::pair{static_cast<char>('a' + static_cast<int>(screenCoords.first / squareSize)),
                     numberOfSquares - static_cast<int>(screenCoords.second / squareSize)};
}

bool Renderer::initialize()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        return false;

    window = SDL_CreateWindow("Chess Engine", boardSize, boardSize, 0);
    if (!window)
    {
        SDL_Quit();
        return false;
    }

    sdlRenderer = SDL_CreateRenderer(window, nullptr);
    if (!sdlRenderer)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    return true;
}

void Renderer::shutdown()
{
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::renderPiece(const Piece& piece)
{
    Position position = piece.getPosition();
    auto coords = convertCoords(position.getPair());

    SDL_FRect pieceRectangle{
        coords.first + 20.0f, coords.second + 20.0f, squareSize - 40.0f, squareSize - 40.0f};

    if (piece.getColor() == Color::White)
    {
        SDL_SetRenderDrawColor(sdlRenderer, 240, 240, 240, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(sdlRenderer, 40, 40, 40, 255);
    }
    SDL_RenderFillRect(sdlRenderer, &pieceRectangle);
}

void Renderer::renderMoveIndicator(Position position)
{
    auto [x, y] = convertCoords(position.getPair());

    SDL_FRect square{x + 37.5f, y + 37.5f, squareSize / 4.0f, squareSize / 4.0f};

    SDL_SetRenderDrawColor(sdlRenderer, 100, 100, 100, 150);
    SDL_RenderFillRect(sdlRenderer, &square);
}

void Renderer::renderSquareBorder(Position position, SDL_Color color, float inset = 0)
{
    Uint8 oldR, oldG, oldB, oldA;
    SDL_GetRenderDrawColor(sdlRenderer, &oldR, &oldG, &oldB, &oldA);

    auto [x, y] = convertCoords(position.getPair());

    SDL_SetRenderDrawColor(sdlRenderer, color.r, color.g, color.b, color.a);
    constexpr float borderWidth = squareSize / 100 * 6;
    for (float i = 0; i < borderWidth; i++)
    {
        SDL_FRect square{x + i + inset,
                         y + i + inset,
                         squareSize - 2.0f * i - inset * 2,
                         squareSize - 2.0f * i - inset * 2};
        SDL_RenderRect(sdlRenderer, &square);
    }

    SDL_SetRenderDrawColor(sdlRenderer, oldR, oldG, oldB, oldA);
}

void Renderer::render(const Board& board)
{
    SDL_SetRenderDrawColor(sdlRenderer, 40, 40, 40, 255);
    SDL_RenderClear(sdlRenderer);

    auto selectedPosition = game.getSelectedPosition();
    auto selectedMoves = game.getSelectedMoves();
    auto checkedKing = board.getCheckedKing();

    for (int row = 0; row < 8; ++row)
    {
        for (int column = 0; column < 8; ++column)
        {
            if ((row + column) % 2 == 0)
                SDL_SetRenderDrawColor(sdlRenderer, 240, 217, 181, 255);
            else
                SDL_SetRenderDrawColor(sdlRenderer, 181, 136, 99, 255);

            SDL_FRect square{column * squareSize, row * squareSize, squareSize, squareSize};

            SDL_RenderFillRect(sdlRenderer, &square);

            Position position = Position::fromValid('a' + column, numberOfSquares - row);
            Piece* piece = board.getPiece(position);

            if (piece)
                renderPiece(*piece);
        }
    }

    for (const Move& move : selectedMoves)
    {
        renderMoveIndicator(move.to);
    }

    if (selectedPosition)
        renderSquareBorder(*selectedPosition, SDL_Color{255, 255, 255, 255});
    if (checkedKing)
    {
        float inset = (checkedKing == selectedPosition) ? 6.0f : 0.0f;
        renderSquareBorder(*checkedKing, SDL_Color{255, 0, 0, 255}, inset);
    }

    SDL_RenderPresent(sdlRenderer);
}