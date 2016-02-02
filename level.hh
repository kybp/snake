#ifndef LEVEL_HH
#define LEVEL_HH

#include <memory>
#include <utility>
#include <vector>
#include "cell.hh"
#include "direction.hh"
#include "snake.hh"

struct SDL_Surface;

class Food : public Cell {
public:
    Food(SDL_Surface *surface, unsigned x, unsigned y)
        : Cell(surface, x, y, SDL_MapRGB(surface->format, 0, 127, 0))
        {}
    Food(SDL_Surface *surface, std::pair<unsigned, unsigned> coords)
        : Food(surface, coords.first, coords.second)
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
    Level(SDL_Surface *surface, unsigned width, unsigned height,
          unsigned *score);
    Level(SDL_Surface *surface, unsigned screenWidth, unsigned screenHeight,
          const char *filename, unsigned *score);
    void draw();
    bool snakeAlive() const;
    Snake &getSnake() const;
    bool snakeWillRunIntoWall() const;
    void update();
    bool invalidFoodPosition(const std::pair<unsigned, unsigned>& coords) const;
private:
    void generateRandomFood();
    unsigned *score;
    SDL_Surface *surface;
    bool alive, winnable, won;
    unsigned width, height;
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
