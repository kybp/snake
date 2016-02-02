#include <cstdlib>
#include <utility>
#include "cell.hh"
#include "direction.hh"

static const Direction UP    = Direction::UP;
static const Direction DOWN  = Direction::DOWN;
static const Direction LEFT  = Direction::LEFT;
static const Direction RIGHT = Direction::RIGHT;

unsigned Cell::h, Cell::w, Cell::yOffset;

Cell::Cell(SDL_Surface *surface, unsigned x, unsigned y, Uint32 color)
    : x(x), y(y), position(new SDL_Rect), surface(surface), color(color)
{
    position->x = x * width();
    position->y = y * height() + yOffset;
    position->w = width();
    position->h = height();
}

std::pair<int, int> Cell::nextPosition(Direction direction) const
{
    switch (direction) {
    case UP:    return { x, y - 1 };
    case DOWN:  return { x, y + 1 };
    case LEFT:  return { x - 1, y };
    case RIGHT: return { x + 1, y };
    }
}

void Cell::move(Direction direction)
{
    auto next = nextPosition(direction);
    x = next.first;
    y = next.second;
    position->x = next.first  * width();
    position->y = next.second * height() + yOffset;
}
