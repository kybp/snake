#include <cstdlib>
#include <utility>
#include "cell.hh"
#include "direction.hh"

static const Direction UP    = Direction::UP;
static const Direction DOWN  = Direction::DOWN;
static const Direction LEFT  = Direction::LEFT;
static const Direction RIGHT = Direction::RIGHT;

unsigned Cell::h, Cell::w;

Cell::Cell(SDL_Surface *surface, unsigned x, unsigned y, Uint32 color)
    : x(x), y(y), position(new SDL_Rect), surface(surface), color(color)
{
    position->x = x * width();
    position->y = y * height() + yOffset;
    position->w = width();
    position->h = height();
}

std::pair<unsigned, unsigned> Cell::nextPosition(Direction direction) const
{
    switch (direction) {
    case UP:    return { y - 1, position->y - height() };
    case DOWN:  return { y + 1, position->y + height() };
    case LEFT:  return { x - 1, position->x - width()  };
    case RIGHT: return { x + 1, position->x + width()  };
    }
}

void Cell::move(Direction direction)
{
    auto next = nextPosition(direction);
    position->x = next.first;
    position->y = next.second;
}
