#include <cstdlib>
#include "cell.hh"
#include "direction.hh"

static const Direction UP    = Direction::UP;
static const Direction DOWN  = Direction::DOWN;
static const Direction LEFT  = Direction::LEFT;
static const Direction RIGHT = Direction::RIGHT;

int Cell::h, Cell::w;

Cell::Cell(SDL_Surface *surface, int cellX, int cellY, Uint32 color):
    cellX(cellX), cellY(cellY), surface(surface), color(color)
{
    position.x = cellX * width();
    position.y = cellY * height();
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
        --cellY;
        position.y -= height();
        break;
    case DOWN:
        ++cellY;
        position.y += height();
        break;
    case LEFT:
        --cellX;
        position.x -= width();
        break;
    case RIGHT:
        ++cellX;
        position.x += width();
        break;
    }
}
