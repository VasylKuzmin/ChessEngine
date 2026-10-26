#include "GUI/Renderer.h"

#include "Board.h"
#include "Game.h"
#include "Pieces/Piece.h"

#include <cmath>
#include <stdexcept>

Renderer::Renderer(Game& game) : game(game)
{
}

Renderer::~Renderer() = default;

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

void Renderer::initialize()
{

    window.reset(
        SDL_CreateWindow("Chess Engine", boardSize + squareSize * 1.5, boardSize + squareSize, 0));
    if (!window)
    {
        throw std::runtime_error(SDL_GetError());
    }

    sdlRenderer.reset(SDL_CreateRenderer(window.get(), nullptr));
    if (!sdlRenderer)
    {
        throw std::runtime_error(SDL_GetError());
    }

    textureHandler = std::make_unique<TextureHandler>(*sdlRenderer);
}

void Renderer::renderPiece(const Piece& piece)
{
    Position position = piece.getPosition();
    auto [x, y] = convertCoords(position.getPair());
    SDL_FRect destination{x, y, squareSize, squareSize};
    SDL_RenderTexture(sdlRenderer.get(),
                      textureHandler->getPieceTexture(piece.getType(), piece.getColor()),
                      nullptr,
                      &destination);
}

void Renderer::renderMoveIndicator(Position position)
{
    auto [x, y] = convertCoords(position.getPair());

    float centerX = x + squareSize / 2.0f;
    float centerY = y + squareSize / 2.0f;
    float radius = squareSize / 8.0f;

    SDL_SetRenderDrawColor(sdlRenderer.get(), 100, 100, 100, 255);

    for (float dy = -radius; dy <= radius; ++dy)
    {
        float width = std::sqrt(radius * radius - dy * dy);

        SDL_RenderLine(
            sdlRenderer.get(), centerX - width, centerY + dy, centerX + width, centerY + dy);
    }
}

void Renderer::renderSquareBorder(Position position, SDL_Color color, float inset = 0)
{
    auto [x, y] = convertCoords(position.getPair());

    SDL_SetRenderDrawColor(sdlRenderer.get(), color.r, color.g, color.b, color.a);
    constexpr float borderWidth = squareSize / 100 * 6;
    for (float i = 0; i < borderWidth; i++)
    {
        SDL_FRect square{x + i + inset,
                         y + i + inset,
                         squareSize - 2.0f * i - inset * 2,
                         squareSize - 2.0f * i - inset * 2};
        SDL_RenderRect(sdlRenderer.get(), &square);
    }
}

void Renderer::render(const Board& board)
{
    SDL_SetRenderDrawColor(sdlRenderer.get(), 40, 40, 40, 255);
    SDL_RenderClear(sdlRenderer.get());

    SDL_SetRenderDrawColor(sdlRenderer.get(), 50, 50, 50, 255);
    SDL_FRect square = {0, squareSize * 8, squareSize * 8, squareSize};
    SDL_RenderFillRect(sdlRenderer.get(), &square);

    square = {squareSize * 8, squareSize * 0, squareSize * 1.5, squareSize * 9};
    SDL_RenderFillRect(sdlRenderer.get(), &square);

    auto selectedPosition = game.getSelectedPosition();
    auto selectedMoves = game.getSelectedMoves();
    auto checkedKing = board.getCheckedKing();

    for (int row = 0; row < 8; ++row)
    {
        for (int column = 0; column < 8; ++column)
        {
            if ((row + column) % 2 == 0)
                SDL_SetRenderDrawColor(sdlRenderer.get(), 240, 217, 181, 255);
            else
                SDL_SetRenderDrawColor(sdlRenderer.get(), 181, 136, 99, 255);

            Position position = Position::fromValid('a' + column, numberOfSquares - row);

            SDL_FRect square{column * squareSize, row * squareSize, squareSize, squareSize};
            SDL_RenderFillRect(sdlRenderer.get(), &square);

            if (row == 7)
            {
                SDL_Texture* texture = textureHandler->getLetterTexture(position.getHorizontal());
                square = {column * squareSize + 110.0f,
                          row * squareSize + 95.0f,
                          static_cast<float>(texture->w),
                          static_cast<float>(texture->h)};
                SDL_RenderTexture(sdlRenderer.get(), texture, nullptr, &square);
            }

            if (column == 0)
            {
                SDL_Texture* texture = textureHandler->getNumberTexture(position.getVertical());
                square = {column * squareSize + 5.0f,
                          row * squareSize,
                          static_cast<float>(texture->w),
                          static_cast<float>(texture->h)};
                SDL_RenderTexture(sdlRenderer.get(), texture, nullptr, &square);
            }

            Piece* piece = board.getPiece(position);

            if (piece)
                renderPiece(*piece);
        }
    }

    for (const Move& move : selectedMoves)
    {
        if (move.type == MoveType::Capture || move.type == MoveType::EnPassant)
            renderSquareBorder(move.to, SDL_Color{220, 130, 60, 255});
        else
            renderMoveIndicator(move.to);
    }

    if (selectedPosition)
        renderSquareBorder(*selectedPosition, SDL_Color{80, 160, 230, 255});
    if (checkedKing)
    {
        float inset = (checkedKing == selectedPosition) ? 6.0f : 0.0f;
        renderSquareBorder(*checkedKing, SDL_Color{210, 80, 80, 255}, inset);
    }

    promotionPositions = game.getPromotionPositions();
    if (promotionPositions[0])
        renderPromotionChoices(game.getCurrentPlayer());

    if (game.getGameState() == GameState::Stalemate || game.getGameState() == GameState::Checkmate)
    {
        SDL_SetRenderDrawColor(sdlRenderer.get(), 215, 210, 202, 255);
        SDL_FRect square{
            squareSize * 2.5 - 10, squareSize * 2.5 - 10, squareSize * 3 + 20, squareSize * 2 + 20};
        SDL_RenderFillRect(sdlRenderer.get(), &square);

        SDL_SetRenderDrawColor(sdlRenderer.get(), 245, 240, 225, 255);
        square = {squareSize * 2.5, squareSize * 2.5, squareSize * 3, squareSize * 2};
        SDL_RenderFillRect(sdlRenderer.get(), &square);

        if (game.getGameState() == GameState::Stalemate)
        {
            SDL_Texture* texture = textureHandler->getStalemateTexture();
            square = {squareSize * 3,
                      squareSize * 3,
                      static_cast<float>(texture->w),
                      static_cast<float>(texture->h)};
            SDL_RenderTexture(sdlRenderer.get(), texture, nullptr, &square);
        }
        else
        {
            SDL_Texture* texture = textureHandler->getCheckmateTexture();
            square = {squareSize * 3,
                      squareSize * 3.5,
                      static_cast<float>(texture->w),
                      static_cast<float>(texture->h)};
            SDL_RenderTexture(sdlRenderer.get(), texture, nullptr, &square);
            Color winnerColor =
                game.getCurrentPlayer() == Color::White ? Color::White : Color::Black;
            SDL_Texture* winnerTexture = winnerColor == Color::White
                                             ? textureHandler->getWhiteWordTexture()
                                             : textureHandler->getBlackWordTexture();
            square = {squareSize * 3,
                      squareSize * 3,
                      static_cast<float>(winnerTexture->w),
                      static_cast<float>(winnerTexture->h)};
            SDL_RenderTexture(sdlRenderer.get(), winnerTexture, nullptr, &square);
        }
    }

    SDL_Texture* texture = textureHandler->getBackButtonTexture();
    square = {0, squareSize * 8, squareSize, squareSize};
    SDL_RenderTexture(sdlRenderer.get(), texture, nullptr, &square);

    auto moveRecord = board.getMoveHistory();

    if (!moveRecord.empty())
    {
        size_t startIndex = (moveRecord.size() > 36) ? moveRecord.size() - 36 : 0;
        for (size_t i = startIndex; i < moveRecord.size(); i++)
        {
            SDL_FRect square = {squareSize * 8.8, squareSize * (i - startIndex) / 4, 32, 32};
            SDL_SetRenderDrawColor(sdlRenderer.get(), 150, 150, 150, 255);
            SDL_RenderFillRect(sdlRenderer.get(), &square);
            SDL_Texture* texture = textureHandler->getPieceTexture(moveRecord[i].movedPiece,
                                                                   moveRecord[i].movedPieceColor);
            SDL_RenderTexture(sdlRenderer.get(), texture, nullptr, &square);

            texture = textureHandler->getWhiteNumberTexture(i + 1);
            square = {squareSize * 8.05,
                      squareSize * (i - startIndex) / 4,
                      static_cast<float>(texture->w),
                      static_cast<float>(texture->h)};
            SDL_RenderTexture(sdlRenderer.get(), texture, nullptr, &square);

            Position position = moveRecord[i].move.from;
            texture = textureHandler->getWhiteLetterTexture(position.getHorizontal());
            square = {squareSize * 8.45,
                      squareSize * (i - startIndex) / 4,
                      static_cast<float>(texture->w),
                      static_cast<float>(texture->h)};
            SDL_RenderTexture(sdlRenderer.get(), texture, nullptr, &square);

            texture = textureHandler->getWhiteNumberTexture(position.getVertical());
            square = {squareSize * 8.6,
                      squareSize * (i - startIndex) / 4,
                      static_cast<float>(texture->w),
                      static_cast<float>(texture->h)};
            SDL_RenderTexture(sdlRenderer.get(), texture, nullptr, &square);

            position = moveRecord[i].move.to;
            texture = textureHandler->getWhiteLetterTexture(position.getHorizontal());
            square = {squareSize * 9.1,
                      squareSize * (i - startIndex) / 4,
                      static_cast<float>(texture->w),
                      static_cast<float>(texture->h)};
            SDL_RenderTexture(sdlRenderer.get(), texture, nullptr, &square);

            texture = textureHandler->getWhiteNumberTexture(position.getVertical());
            square = {squareSize * 9.25,
                      squareSize * (i - startIndex) / 4,
                      static_cast<float>(texture->w),
                      static_cast<float>(texture->h)};
            SDL_RenderTexture(sdlRenderer.get(), texture, nullptr, &square);
        }
    }

    SDL_RenderPresent(sdlRenderer.get());
}

void Renderer::renderPromotionChoices(Color color)
{
    PieceType promotionTypes[] = {
        PieceType::Queen, PieceType::Rook, PieceType::Bishop, PieceType::Knight};
    SDL_SetRenderDrawColor(sdlRenderer.get(), 230, 230, 230, 255);

    size_t i = 0;
    for (const auto& position : promotionPositions)
    {
        auto [x, y] = convertCoords(position.value().getPair());
        SDL_FRect square{x, y, squareSize, squareSize};
        SDL_RenderFillRect(sdlRenderer.get(), &square);

        SDL_Texture* texture = textureHandler->getPieceTexture(promotionTypes[i], color);
        auto destination = convertCoords(promotionPositions[i].value().getPair());
        SDL_FRect location{destination.first, destination.second, squareSize, squareSize};
        SDL_RenderTexture(sdlRenderer.get(), texture, nullptr, &location);

        i++;
    }
}

float Renderer::getSquareSize() const
{
    return squareSize;
}