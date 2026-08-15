#include "Board.h"
#include "GUI/Renderer.h"
#include "Position.h"
#include "Game.h"

#include <SDL3/SDL.h>
#include <utility>

int main()
{
    Board board;
    board.setUpDefault();
    
    Game game;

    Renderer renderer(game);
    if (!renderer.initialize())
        return 1;

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
                Position clickPosition = Position::fromValid(
                    Renderer::convertCoords(std::pair{event.button.x, event.button.y}));
                game.handleClick(board, clickPosition);
            }

        }

        renderer.render(board);
    }

    renderer.shutdown();

    return 0;
}
