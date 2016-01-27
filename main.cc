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
    void draw();
    void generateFood();
    void gameOver();
    bool handleKey(const SDL_Keycode keycode);
    void mainLoop();
    void run();
    void update();
private:
    Cell *food;
    Uint32 foodColor;
    const int heightInCells;
    const int widthInCells;
    const Uint32 maxDelay = 200;
    bool paused = false;
    bool running = true;
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
    running = false;
}

void SnakeGame::generateFood()
{
    do {
        int x = (rand() % widthInCells) * Cell::width;
        int y = (rand() % heightInCells) * Cell::height;
        food = new Cell(surface, x, y, foodColor);
    } while (snake->collidesWith(*food) && (delete food, true));
}

bool SnakeGame::handleKey(const SDL_Keycode keycode)
{
    bool quit = false;

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
            running = false;
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
            running = false;
            break;
        case SDLK_ESCAPE:
            quit = true;
            break;
        }
    }

    return quit;
}

void SnakeGame::mainLoop()
{
    bool quit = false;
    Uint32 lastFrame = SDL_GetTicks();
    while (!quit && running) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                quit = handleKey(e.key.keysym.sym);
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

void SnakeGame::run()
{
    mainLoop();
}

inline void SnakeGame::update()
{
    if (snake->move() || snake->xPosition() < 0 || snake->yPosition() < 0 ||
        snake->xPosition() / Cell::width >= widthInCells ||
        snake->yPosition() / Cell::height >= heightInCells) {
        running = false;
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
    SnakeGame(window, surface, w, h, initialLength).run();
    SDL_DestroyWindow(window);
    SDL_Quit();
}
