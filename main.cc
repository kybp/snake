#include <cstdlib>
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
    void draw();
    void generateFood();
    void gameOver();
    bool handleKey(const SDL_Keycode keycode);
    void mainLoop();
    void run();
    void update();
    static const int windowWidth  = 640;
    static const int windowHeight = 480;
    Uint32 maxDelay = 200;
private:
    Cell *food;
    Uint32 foodColor;
    static const int maxX = windowWidth  / Cell::width;
    static const int maxY = windowHeight / Cell::height;
    bool paused = false;
    bool running = true;
    int score;
    Snake *snake;
    SDL_Surface *surface;
    SDL_Window *window;
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
    foodColor = SDL_MapRGB(surface->format, 255, 0, 0);
    generateFood();
}

SnakeGame::~SnakeGame()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
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
    cout << "You died with " << score << " points" << endl;
    running = false;
}

void SnakeGame::generateFood()
{
    do {
        int x = (rand() % maxX) * Cell::width;
        int y = (rand() % maxY) * Cell::height;
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
        snake->xPosition() / Cell::width >= maxX ||
        snake->yPosition() / Cell::height >= maxY) {
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
    SnakeGame().run();
}
