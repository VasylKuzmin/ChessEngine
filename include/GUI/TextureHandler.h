#pragma once

#include "Color.h"
#include "PieceType.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <array>
#include <memory>
#include <string>
#include <utility>
#include <vector>

struct TextureDeleter
{
    void operator()(SDL_Texture* texture) const
    {
        SDL_DestroyTexture(texture);
    }
};

using TexturePtr = std::unique_ptr<SDL_Texture, TextureDeleter>;

class TextureHandler
{
  public:
    explicit TextureHandler(SDL_Renderer&);
    SDL_Texture* getPieceTexture(PieceType, Color);
    SDL_Texture* getLetterTexture(char);
    SDL_Texture* getWhiteLetterTexture(char);
    SDL_Texture* getNumberTexture(int);
    SDL_Texture* getWhiteNumberTexture(int);
    void addNextWhiteNumberTexture();
    SDL_Texture* getStalemateTexture();
    SDL_Texture* getCheckmateTexture();
    SDL_Texture* getWhiteWordTexture();
    SDL_Texture* getBlackWordTexture();
    SDL_Texture* getBackButtonTexture();

  private:
    SDL_Renderer& renderer;

    std::array<std::array<TexturePtr, 6>, 2> pieceTextures;

    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font{nullptr, TTF_CloseFont};
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> fontBig{nullptr, TTF_CloseFont};
    std::array<TexturePtr, 8> letterTextures;
    std::array<TexturePtr, 8> whiteLetterTextures;
    std::array<TexturePtr, 8> numberTextures;
    std::vector<TexturePtr> whiteNumberTextures;
    TexturePtr stalemateTexture;
    TexturePtr checkmateTexture;
    TexturePtr whiteWordTexture;
    TexturePtr blackWordTexture;
    TexturePtr backButtonTexture;

    SDL_Texture* textTexture(const std::string&, SDL_Color);
    TexturePtr loadTexture(const char* path);
};