#include "Board.h"
#include "GUI/Renderer.h"
#include "Game.h"
#include "Position.h"

#include <SDL3/SDL.h>
#include <utility>

int main()
{
    try
    {
        Board board;
        board.setUpDefault();
        Game game;
        Renderer renderer(game);
        renderer.initialize();

        bool running = true;

        while (running)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT)
                    running = false;
                if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                {
                    auto coords = std::pair{static_cast<float>(event.button.x),
                                            static_cast<float>(event.button.y)};
                    auto positionCoords =
                        Renderer::convertCoords(coords);
                    std::optional<Position> clickPosition =
                        Position::tryCreate(positionCoords.first, positionCoords.second);
                    if (clickPosition)
                        game.handlePositionClick(board, clickPosition);
                    else
                        game.handleClick(board, coords, renderer.getSquareSize());
                }
            }
            renderer.render(board);
        }
    }

    catch (const std::exception& e)
    {
        SDL_Log("Error: %s", e.what());
        return 1;
    }

    return 0;
}
