#include <cstdlib>
#include <iostream>
#include <SDL.h>
#include "direction.hh"
#include "snake.hh"

using Direction::UP;
using Direction::DOWN;
using Direction::LEFT;
using Direction::RIGHT;

class SnakeGame {
public:
    SnakeGame(SDL_Window *winodw, SDL_Surface *surface,
              int widthInCells, int heightInCells, int initialLength);
    bool didQuit();
    void draw();
    void generateFood();
    void gameOver();
    void handleKey(const SDL_Keycode keycode);
    void run();
    void update();
private:
    Cell *food;
    Uint32 foodColor;
    const int heightInCells;
    const int widthInCells;
    const Uint32 maxDelay = 200;
    bool alive  = true;
    bool paused = false;
    bool quit   = false;
    int score;
    Snake *snake;
    SDL_Surface *surface;
    SDL_Window *window;
};

SnakeGame::SnakeGame(SDL_Window *window, SDL_Surface *surface,
                     int widthInCells, int heightInCells,
                     int initialLength) :
    heightInCells(heightInCells), widthInCells(widthInCells),
    score(0), surface(surface), window(window)
{
    int pixelHeight = heightInCells * Cell::height;
    int pixelWidth  = widthInCells  * Cell::width;
    snake = new Snake(surface, pixelWidth / 2, pixelHeight / 2, LEFT,
                      pixelWidth, pixelHeight, initialLength);
    foodColor = SDL_MapRGB(surface->format, 255, 0, 0);
    generateFood();
}

inline bool SnakeGame::didQuit()
{
    return quit;
}

inline void SnakeGame::draw()
{
    SDL_FillRect(surface, nullptr, 0);
    food->draw();
    snake->draw();
    SDL_UpdateWindowSurface(window);
}

void SnakeGame::gameOver()
{
    // This is a placeholder. Eventually it will prompt the user to
    // press a button to start a new game.
    std::cout << "You died with " << score << " points" << std::endl;
    alive = false;
}

void SnakeGame::generateFood()
{
    do {
        int x = (rand() % widthInCells) * Cell::width;
        int y = (rand() % heightInCells) * Cell::height;
        food = new Cell(surface, x, y, foodColor);
    } while (snake->collidesWith(*food) && (delete food, true));
}

void SnakeGame::handleKey(const SDL_Keycode keycode)
{
    if (!paused) {
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
        case SDLK_p:
            paused = true;
            break;
        case SDLK_q:
            quit = true;
            break;
        case SDLK_ESCAPE:
            quit = true;
            break;
        case SDLK_g:
            snake->grow();
            break;
        }
    } else {
        switch (keycode) {
        case SDLK_p:
            paused = false;
            break;
        case SDLK_q:
            quit = true;
            break;
        case SDLK_ESCAPE:
            quit = true;
            break;
        }
    }
}

void SnakeGame::run()
{
    Uint32 lastFrame = SDL_GetTicks();
    while (!quit && alive) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                handleKey(e.key.keysym.sym);
                break;
            }
        }
        if (SDL_GetTicks() - lastFrame >= maxDelay && !paused) {
            update();
            draw();
            lastFrame = SDL_GetTicks();
        }
    }
    if (!quit) gameOver();
}

inline void SnakeGame::update()
{
    if (snake->move() || snake->xPosition() < 0 || snake->yPosition() < 0 ||
        snake->xPosition() / Cell::width >= widthInCells ||
        snake->yPosition() / Cell::height >= heightInCells) {
        alive = false;
    } else if (snake->collidesWith(*food)) {
        delete food;
        generateFood();
        snake->grow();
        ++score;
    }
}

int main(void)
{
    int w = 20, h = 20, initialLength = 4;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Could not initialize SDL." << std::endl;
        exit(1);
    }

    SDL_Window *window = SDL_CreateWindow(
        "Snake",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        w * Cell::width, h * Cell::height,
        SDL_WINDOW_SHOWN);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    for (;;) {
        auto game = SnakeGame(window, surface, w, h, initialLength);
        game.run();
        if (game.didQuit()) break;
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}
