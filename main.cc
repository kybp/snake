#include <iostream>
#include <SDL.h>
#include "direction.hh"
#include "snake.hh"

using namespace std;
using Direction::UP;
using Direction::DOWN;
using Direction::LEFT;
using Direction::RIGHT;

class SnakeGame {
public:
    SnakeGame();
    ~SnakeGame();
    void close();
    bool handleKey(const SDL_Keycode keycode);
    void mainLoop();
    void run();
    static const int windowWidth  = 640;
    static const int windowHeight = 480;
private:
    Snake *snake;
    SDL_Window *window;
    SDL_Surface *surface;
};

SnakeGame::SnakeGame()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "Could not initialize SDL." << endl;
        exit(1);
    }
    window = SDL_CreateWindow("Snake",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(window);
    snake = new Snake(surface, windowWidth / 2, windowHeight / 2, LEFT);
}

SnakeGame::~SnakeGame()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool SnakeGame::handleKey(const SDL_Keycode keycode)
{
    bool quit = false;

    switch (keycode) {
    case SDLK_UP:
        snake->changeDirection(UP);
        break;
    case SDLK_DOWN:
        snake->changeDirection(DOWN);
        break;
    case SDLK_LEFT:
        snake->changeDirection(LEFT);
        break;
    case SDLK_RIGHT:
        snake->changeDirection(RIGHT);
        break;
    case SDLK_q: case SDLK_ESCAPE:
        quit = true;
        break;
    case SDLK_SPACE:
        snake->move();
        break;
        // Debug
    case SDLK_g:
        snake->grow();
        break;
    }

    return quit;
}

void SnakeGame::mainLoop()
{
    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                quit = handleKey(e.key.keysym.sym);
                break;
            }
        }
        SDL_FillRect(surface, nullptr, 0);
        snake->draw();
        SDL_UpdateWindowSurface(window);
    }
}

void SnakeGame::run()
{
    mainLoop();
}

int main(void)
{
    SnakeGame().run();
}
