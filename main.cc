#include <iostream>
#include <sstream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "direction.hh"
#include "level.hh"

class SnakeGame {
public:
    SnakeGame(SDL_Window *window, unsigned width, unsigned height,
              TTF_Font *font);
    SnakeGame(SDL_Window *window, unsigned width, unsigned height,
              TTF_Font *font, const char *filename);
    void run();
private:
    static const Uint32 maxDelay = 200;
    void drawScore();
    void gameOver();
    void handleKey(SDL_Keycode keycode);
    Uint32 Uint32FromSDL_Color(SDL_Color color);
    bool paused, running;
    unsigned score, highScore;
    SDL_Window *window;
    SDL_Surface *surface;
    TTF_Font *font;
    static const SDL_Color scoreColor, scoreBackgroundColor;
    std::unique_ptr<Level> level;
};

const SDL_Color SnakeGame::scoreColor = { 0xff, 0xff, 0xff, 0 };
const SDL_Color SnakeGame::scoreBackgroundColor = { 127, 0, 0, 0 };

SnakeGame::SnakeGame(SDL_Window *window, unsigned width, unsigned height,
                     TTF_Font *font)
    : paused(false), running(true), score(0), highScore(0),
      window(window), surface(SDL_GetWindowSurface(window)),
      font(font)
{
    level = std::unique_ptr<Level>(
        new Level(surface, width, height, &score, &highScore));
}

SnakeGame::SnakeGame(SDL_Window *window, unsigned width, unsigned height,
                     TTF_Font *font, const char *filename)
    : paused(false), running(true), score(0), highScore(0),
      window(window), surface(SDL_GetWindowSurface(window)),
      font(font)
{
    level = std::unique_ptr<Level>(
        new Level(surface, width, height, filename, &score, &highScore));
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

Uint32 SnakeGame::Uint32FromSDL_Color(SDL_Color color)
{
    return SDL_MapRGB(surface->format, color.r, color.g, color.b);
}

void SnakeGame::run()
{
    Uint32 lastFrame = SDL_GetTicks();
    while (running && !level->didWin()) {
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

        if (SDL_GetTicks() - lastFrame >= maxDelay) {
            if (!paused) level->update();
            level->draw();
            drawScore();
            if (!level->snakeAlive()) level->reset();
            SDL_UpdateWindowSurface(window);
            lastFrame = SDL_GetTicks();
        }
    }
}

void SnakeGame::drawScore()
{
    static const SDL_Rect margin = { 0, 0, surface->w, TTF_FontHeight(font) };
    SDL_FillRect(surface, &margin, Uint32FromSDL_Color(scoreBackgroundColor));
    std::ostringstream scoreDisplay;
    const char *separator = "      ";
    scoreDisplay << " Score: " << score;
    scoreDisplay << separator << "High score: " << highScore;
    if (paused) scoreDisplay << separator << "(paused)";
    const char *scoreString = scoreDisplay.str().c_str();
    SDL_Surface *scoreSurface = TTF_RenderText_Shaded(font, scoreString,
                                                      scoreColor,
                                                      scoreBackgroundColor);
    if (scoreSurface == nullptr) {
        std::cerr << "Error writing score." << std::endl;
        exit(1);
    }

    SDL_BlitSurface(scoreSurface, nullptr, surface, nullptr);
    SDL_FreeSurface(scoreSurface);
}

int main(int argc, char **argv)
{
    const unsigned w = 20, h = 15;
    Cell::setWidth(40);
    Cell::setHeight(40);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Could not initialize SDL." << std::endl;
        exit(1);
    } else if (TTF_Init() < 0) {
        std::cerr << "Could not initialize SDL_ttf." << std::endl;
        exit(1);
    }

    const char *fontName    = "FreeSans.ttf";
    const int fontPointSize = 30;
    TTF_Font *font = TTF_OpenFont(fontName, fontPointSize);
    Cell::setYOffset(TTF_FontHeight(font));

    const unsigned screenWidth  = w * Cell::width();
    const unsigned screenHeight = h * Cell::height() + TTF_FontHeight(font);

    SDL_Window *window = SDL_CreateWindow(
        "Snake",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        screenWidth,             screenHeight,
        SDL_WINDOW_SHOWN);

    if (argc == 1) {
        SnakeGame(window, w, h, font).run();
    } else {
        while (--argc) {
            SnakeGame(window, screenWidth, screenHeight, font, *++argv).run();
        }
    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_Quit();
}
