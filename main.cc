#include <cstdlib>
#include <iostream>
#include <utility>
#include <SDL.h>
#include "direction.hh"
#include "food.hh"
#include "layout.hh"
#include "snake.hh"

static const Direction UP    = Direction::UP;
static const Direction DOWN  = Direction::DOWN;
static const Direction LEFT  = Direction::LEFT;
static const Direction RIGHT = Direction::RIGHT;

class SnakeGame {
public:
    SnakeGame(SDL_Window *winodw, int widthInCells, int heightInCells,
              int initialLength);
    SnakeGame(SDL_Window *window, Layout *layout, bool generateStartingFood);
    bool didLose();
    bool didQuit();
    void winGame();
    void loseGame();
    void quitGame();
    void draw();
    void generateFood();
    void handleKey(const SDL_Keycode keycode);
    bool isRunning();
    void run();
    void update();
private:
    const int heightInCells;
    const int widthInCells;
    const Uint32 maxDelay = 200;
    bool alive   = true;
    bool paused  = false;
    bool quit    = false;
    bool running = true;
    int score;
    SDL_Surface *surface;
    SDL_Window *window;
    Layout *layout;
    Snake *snake;
};

SnakeGame::SnakeGame(SDL_Window *window, int widthInCells, int heightInCells,
                     int initialLength):
    heightInCells(heightInCells), widthInCells(widthInCells), score(0),
    surface(SDL_GetWindowSurface(window)), window(window),
    layout(new Layout(surface, heightInCells, widthInCells))
{
    int pixelHeight = heightInCells * Cell::height();
    int pixelWidth  = widthInCells  * Cell::width();
    snake = new Snake(surface, widthInCells / 2, heightInCells / 2, LEFT,
                      pixelWidth, pixelHeight, initialLength);
    generateFood();
}

SnakeGame::SnakeGame(SDL_Window *window, Layout *layout,
    bool generateStartingFood)
    : heightInCells(layout->getHeightInCells()),
      widthInCells(layout->getWidthInCells()),
      score(0), surface(SDL_GetWindowSurface(window)),
      window(window), layout(layout)
{
    snake = new Snake(surface,
                      layout->getStartingXCell(),
                      layout->getStartingYCell(),
                      layout->getStartingDirection());
    if (!layout->isWinnable() && generateStartingFood) generateFood();
}

inline bool SnakeGame::didLose()
{
    return !alive;
}

inline bool SnakeGame::didQuit()
{
    return quit;
}

inline bool SnakeGame::isRunning()
{
    return running;
}

inline void SnakeGame::draw()
{
    SDL_FillRect(surface, nullptr, 0);
    layout->draw();
    snake->draw();
    SDL_UpdateWindowSurface(window);
}

void SnakeGame::winGame()
{
    running = false;
}

void SnakeGame::loseGame()
{
    std::cout << "You died with " << score << " points" << std::endl;
    alive = false;
}

void SnakeGame::quitGame()
{
    quit = true;
    running = false;
}

void SnakeGame::generateFood()
{
    std::pair<int, int> coordinates;
    do {
        coordinates.first  = rand() % widthInCells;
        coordinates.second = rand() % heightInCells;
    } while (snake->collidesWithCells(coordinates) ||
             layout->containsCellCoordinates(coordinates));
    layout->add_food_at(coordinates);
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
        case SDLK_q: case SDLK_ESCAPE:
            quitGame();
            break;
        }
    } else {
        switch (keycode) {
        case SDLK_p:
            paused = false;
            break;
        case SDLK_q: case SDLK_ESCAPE:
            quitGame();
            break;
        }
    }
}

void SnakeGame::run()
{
    Uint32 lastFrame = SDL_GetTicks();
    while (running && alive) {
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
}

inline void SnakeGame::update()
{
    if (snake->move() ||
        snake->xPositionInCells() < 0 ||
        snake->yPositionInCells() < 0 ||
        snake->xPositionInCells() >= widthInCells ||
        snake->yPositionInCells() >= heightInCells ||
        layout->containsCellCoordinates(snake->xPositionInCells(),
                                        snake->yPositionInCells())) {
        loseGame();
    } else if (layout->eat_food_at(snake->xPositionInPixels(),
                                   snake->yPositionInPixels())) {
        if (layout->isWinnable() && layout->noFoodLeft()) winGame();
        if (!layout->isWinnable()) generateFood();
        snake->grow();
        ++score;
    }
}

int main(int argc, char **argv)
{
    int initialLength = 4;
    int cellsWidth    = 20;
    int cellsHeight   = 15;
    Cell::setWidth(40);
    Cell::setHeight(40);
    int screenHeight = cellsHeight * Cell::height();
    int screenWidth  = cellsWidth  * Cell::width();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Could not initialize SDL." << std::endl;
        exit(1);
    }

    SDL_Window *window = SDL_CreateWindow(
        "Snake",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        screenWidth, screenHeight,
        SDL_WINDOW_SHOWN);
    if (argc == 1) {
        for (;;) {
            auto game = SnakeGame(window, cellsWidth, cellsHeight,
                                  initialLength);
            game.run();
            if (game.didQuit()) break;
        }
    } else {
        SDL_Surface *surface = SDL_GetWindowSurface(window);
        while (--argc) {
            std::string filename = *++argv;
            Layout layout(surface, filename);
            Cell::setWidth(screenWidth   / layout.getWidthInCells());
            Cell::setHeight(screenHeight / layout.getHeightInCells());
            layout.updatePosition();
            bool lost = false, running, quit;
            do {
                auto game = SnakeGame(window, &layout, !lost);
                game.run();
                lost    = game.didLose();
                running = game.isRunning();
                quit    = game.didQuit();
            } while (running && lost);
            if (quit) break;
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}
