#ifndef SNAKE_HH
#define SNAKE_HH

#include <vector>
#include <SDL.h>
#include "cell.hh"
#include "direction.hh"

class Snake {
private:
    std::vector<Cell> body;
    Direction direction;
    bool needsToGrow;
    SDL_Surface *surface;
public:
    Snake(SDL_Surface *surface, int cellX, int cellY, Direction direction);
    Snake(SDL_Surface *surface, int cellX, int cellY, Direction direction,
          int screenWidth, int screenHeight,
          decltype(body.size()) initialLength);
    void changeDirection(Direction direction);
    bool collidesWith(const Cell& cell) const;
    void draw() const;
    // grow() will cause the snake to grow by one cell after the next
    // call to move()
    void grow();
    // move() returns true or false to indicate whether that move
    // caused the snake to run into itself
    const Cell& head() const;
    bool move();
    int xPosition() const;
    int yPosition() const;
private:
    Uint32 getColor() const;
    void growToInitialLength(decltype(body.size()) initialLength,
                             int screenWidth, int screenHeight);
};

inline Uint32 Snake::getColor() const
{
    return SDL_MapRGB(surface->format, 0, 127, 127);
}

inline void Snake::grow()
{
    needsToGrow = true;
}

inline const Cell& Snake::head() const
{
    return body.back();
}

inline int Snake::xPosition() const
{
    return body.back().xPositionInPixels();
}

inline int Snake::yPosition() const
{
    return body.back().yPositionInPixels();
}

#endif
