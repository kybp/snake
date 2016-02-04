#include <iostream>
#include <SDL.h>
#include "direction.hh"
#include "level.hh"

class SnakeGame {
public:
    SnakeGame(SDL_Window *window, unsigned width, unsigned height);
    SnakeGame(SDL_Window *window, unsigned width, unsigned height,
              const char *filename);
    void run();
private:
    static const Uint32 maxDelay = 200;
    void gameOver();
    void handleKey(SDL_Keycode keycode);
    bool paused, running;
    unsigned score;
    SDL_Window *window;
    SDL_Surface *surface;
    std::unique_ptr<Level> level;
};

SnakeGame::SnakeGame(SDL_Window *window, unsigned width, unsigned height)
    : paused(false), running(true), score(0),
      window(window), surface(SDL_GetWindowSurface(window))
{
    level = std::unique_ptr<Level>(
        new Level(surface, width, height, &score));
}

SnakeGame::SnakeGame(SDL_Window *window, unsigned width, unsigned height,
                     const char *filename)
    : paused(false), running(true), score(0),
      window(window), surface(SDL_GetWindowSurface(window))
{
    level = std::unique_ptr<Level>(
        new Level(surface, width, height, filename, &score));
}

inline void SnakeGame::gameOver()
{
    running = false;
}

void SnakeGame::handleKey(SDL_Keycode keycode)
{
    if (!paused) {
        switch (keycode) {
        case SDLK_UP:
            level->getSnake().changeDirection(Direction::UP);
            break;
        case SDLK_DOWN:
            level->getSnake().changeDirection(Direction::DOWN);
            break;
        case SDLK_LEFT:
            level->getSnake().changeDirection(Direction::LEFT);
            break;
        case SDLK_RIGHT:
            level->getSnake().changeDirection(Direction::RIGHT);
            break;
        case SDLK_p:
            paused = true;
            break;
        case SDLK_q: case SDLK_ESCAPE:
            gameOver();
            break;
        }
    } else {
        switch (keycode) {
        case SDLK_p:
            paused = false;
            break;
        case SDLK_q: case SDLK_ESCAPE:
            gameOver();
            break;
        }
    }
}

void SnakeGame::run()
{
    Uint32 lastFrame = SDL_GetTicks();
    while (running && level->snakeAlive()) {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0) {
            switch (e.type) {
            case SDL_QUIT:
                gameOver();
                break;
            case SDL_KEYDOWN:
                handleKey(e.key.keysym.sym);
                break;
            }
        }

        if (SDL_GetTicks() - lastFrame >= maxDelay && !paused) {
            level->update();
            level->draw();
            SDL_UpdateWindowSurface(window);
            lastFrame = SDL_GetTicks();
        }
    }
}

int main(int argc, char **argv)
{
    unsigned w = 20, h = 15;    // in cells
    Cell::setWidth(40);
    Cell::setHeight(40);
    unsigned screenWidth  = w * Cell::width();
    unsigned screenHeight = h * Cell::height();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Could not initialize SDL." << std::endl;
        exit(1);
    }

    SDL_Window *window = SDL_CreateWindow(
        "Snake",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        screenWidth,             screenHeight,
        SDL_WINDOW_SHOWN);

    if (argc == 1) {
        SnakeGame(window, w, h).run();
    } else {
        while (--argc) {
            SnakeGame(window, screenWidth, screenHeight, *++argv)
                .run();
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}
