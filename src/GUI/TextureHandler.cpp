#include "GUI/TextureHandler.h"

#include "Color.h"
#include "PieceType.h"

#include <cstddef>
#include <stdexcept>
#include <string>

TextureHandler::TextureHandler(SDL_Renderer& renderer) : renderer(renderer)
{
    for (std::size_t i = 0; i < pieceTextures.size(); i++)
    {
        for (std::size_t in = 0; in < pieceTextures[i].size(); in++)
        {
            std::string path = std::string(ASSETS_PATH "/Pieces/") +
                               std::string(colorNames[i]) + std::string(pieceTypeNames[in]) +
                               ".png";
            
            
            pieceTextures[i][in] = loadTexture(path.c_str());
        }
    }

    backButtonTexture = loadTexture(ASSETS_PATH "/Buttons/backButton.png");

    font.reset(TTF_OpenFont(ASSETS_PATH "/Fonts/defaultFont.ttf", 26));
    if (!font)
    {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_Color darkSquareColor = {181, 136, 99, 255};
    SDL_Color lightSquareColor = {240, 217, 181, 255};

    for (char letter = 'a'; letter <= 'h'; ++letter)
    {
        SDL_Color color = (letter - 'a') % 2 == 0 ? lightSquareColor : darkSquareColor;
        letterTextures[letter - 'a'].reset(textTexture(std::string(1, letter), color));
    }

    for (char number = '1'; number <= '8'; ++number)
    {
        SDL_Color color = (number - '1') % 2 == 0 ? lightSquareColor : darkSquareColor;
        numberTextures[number - '1'].reset(textTexture(std::string(1, number), color));
    }

    SDL_Color color = {255, 255, 255, 255};

    for (char letter = 'a'; letter <= 'h'; ++letter)
    {
        whiteLetterTextures[letter - 'a'].reset(textTexture(std::string(1, letter), color));
    }

    for (char number = '1'; number <= '8'; ++number)
    {
        TexturePtr texture(textTexture(std::string(1, number), color));
        whiteNumberTextures.push_back(std::move(texture));
    }

    stalemateTexture.reset(textTexture("Stalemate", {57, 52, 47, 255}));
    checkmateTexture.reset(textTexture("won by checkmate", {57, 52, 47, 255}));
    whiteWordTexture.reset(textTexture("White", {57, 52, 47, 255}));
    blackWordTexture.reset(textTexture("Black", {57, 52, 47, 255}));
}

SDL_Texture* TextureHandler::getPieceTexture(PieceType type, Color color)
{
    return pieceTextures[static_cast<std::size_t>(color)][static_cast<std::size_t>(type)].get();
}

SDL_Texture* TextureHandler::textTexture(const std::string& text, SDL_Color color)
{
    if (!font)
        return nullptr;
    SDL_Surface* surface = TTF_RenderText_Blended(font.get(), text.c_str(), 0, color);
    if (!surface)
        return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(&renderer, surface);
    SDL_DestroySurface(surface);
    if (!texture)
        return nullptr;
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    return texture;
}

void TextureHandler::addNextWhiteNumberTexture()
{
    whiteNumberTextures.push_back(std::move(TexturePtr(
        textTexture(std::to_string(whiteNumberTextures.size() + 1), {255, 255, 255, 255}))));
}

TexturePtr TextureHandler::loadTexture(const char* path)
{
    TexturePtr texture(IMG_LoadTexture(&renderer, path));

    if (!texture)
        throw std::runtime_error(std::string("Failed to load texture: ") + path + ": " +
                                 SDL_GetError());

    if (!SDL_SetTextureScaleMode(texture.get(), SDL_SCALEMODE_NEAREST))
        throw std::runtime_error(std::string("Failed to set texture scale mode: ") + path + ": " +
                                 SDL_GetError());

    return texture;
}

SDL_Texture* TextureHandler::getLetterTexture(char character)
{
    if (character >= 'a' && character <= 'h')
    {
        return letterTextures[character - 'a'].get();
    }

    return nullptr;
}

SDL_Texture* TextureHandler::getWhiteLetterTexture(char character)
{
    if (character >= 'a' && character <= 'h')
    {
        return whiteLetterTextures[character - 'a'].get();
    }

    return nullptr;
}

SDL_Texture* TextureHandler::getNumberTexture(int number)
{
    if (number >= 1 && number <= 8)
    {
        return numberTextures[number - 1].get();
    }

    return nullptr;
}

SDL_Texture* TextureHandler::getWhiteNumberTexture(int number)
{
    if (number > static_cast<int>(whiteNumberTextures.size()))
    {
        addNextWhiteNumberTexture();
    }
    return whiteNumberTextures[number - 1].get();
}

SDL_Texture* TextureHandler::getStalemateTexture()
{
    return stalemateTexture.get();
}

SDL_Texture* TextureHandler::getCheckmateTexture()
{
    return checkmateTexture.get();
}

SDL_Texture* TextureHandler::getWhiteWordTexture()
{
    return whiteWordTexture.get();
}

SDL_Texture* TextureHandler::getBlackWordTexture()
{
    return blackWordTexture.get();
}

SDL_Texture* TextureHandler::getBackButtonTexture()
{
    return backButtonTexture.get();
}