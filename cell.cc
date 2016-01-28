#include <cstdlib>
#include "cell.hh"
#include "direction.hh"

using Direction::UP;
using Direction::DOWN;
using Direction::LEFT;
using Direction::RIGHT;

int Cell::h, Cell::w;

Cell::Cell(SDL_Surface *surface, int x, int y, Uint32 color):
    x(x), y(y), surface(surface), color(color)
{
    position.x = x;
    position.y = y;
    position.w = width();
    position.h = height();
}

int Cell::height()
{
    return h;
}

int Cell::width()
{
    return w;
}

void Cell::move(Direction direction)
{
    switch (direction) {
    case UP:
        --y;
        position.y -= height();
        break;
    case DOWN:
        ++y;
        position.y += height();
        break;
    case LEFT:
        --x;
        position.x -= width();
        break;
    case RIGHT:
        ++x;
        position.x += width();
        break;
    }
}
