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
    Snake(SDL_Surface *surface, int x, int y, Direction direction,
          int screenWidth, int screenHeight,
          decltype(body.size()) initialLength);
    void changeDirection(Direction direction);
    bool collidesWith(const Cell& cell);
    void draw();
    // grow() will cause the snake to grow by one cell after the next
    // call to move()
    void grow();
    // move() returns true or false to indicate whether that move
    // caused the snake to run into itself
    bool move();
    int xPosition();
    int yPosition();
private:
    void growToInitialLength(decltype(body.size()) initialLength,
                             int screenWidth, int screenHeight);
};

inline void Snake::grow()
{
    needsToGrow = true;
}

inline int Snake::xPosition()
{
    return body.back().xPosition();
}

inline int Snake::yPosition()
{
    return body.back().yPosition();
}

#endif
