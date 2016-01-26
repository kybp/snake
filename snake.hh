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
    Snake(SDL_Surface *surface, int x, int y, Direction direction);
    void changeDirection(Direction direction);
    bool collidesWith(const Cell& cell);
    void draw();
    void grow();
    bool move();
    int xPosition();
    int yPosition();
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
