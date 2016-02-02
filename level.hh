#ifndef LAYOUT_HH
#define LAYOUT_HH

#include <memory>
#include <utility>
#include <vector>
#include "cell.hh"
#include "direction.hh"
#include "snake.hh"

struct SDL_Surface;

class Food : public Cell {
public:
    Food(SDL_Surface *surface, int x, int y)
        : Cell(surface, x, y, SDL_MapRGB(surface->format, 0, 127, 0))
        {}
};

class Wall : public Cell {
public:
    Wall(SDL_Surface *surface, unsigned x, unsigned y)
        : Cell(surface, x, y, SDL_MapRGB(surface->format, 127, 0, 0))
        {}
};

class Level {
public:
    Level(SDL_Surface *surface, unsigned height, unsigned width, int *score);
    void draw();
    bool snakeAlive() const;
    Snake &getSnake() const;
    bool snakeWillRunIntoWall() const;
    void update();
private:
    void generateRandomFood();
    int *score;
    SDL_Surface *surface;
    bool alive;
    unsigned height, width;
    std::unique_ptr<Snake> snake;
    std::vector<std::unique_ptr<Food>> food;
    std::vector<std::unique_ptr<Wall>> walls;
};

inline Snake &Level::getSnake() const
{
    return *snake;
}

inline bool Level::snakeAlive() const
{
    return alive;
}

#endif
