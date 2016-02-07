#include <utility>
#include "cell.hh"
#include "direction.hh"

namespace {
    const Direction UP    = Direction::UP;
    const Direction DOWN  = Direction::DOWN;
    const Direction LEFT  = Direction::LEFT;
    const Direction RIGHT = Direction::RIGHT;
}

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
    std::pair<int, int> result;

    switch (direction) {
    case UP:    result = { x, y - 1 }; break;
    case DOWN:  result = { x, y + 1 }; break;
    case LEFT:  result = { x - 1, y }; break;
    case RIGHT: result = { x + 1, y }; break;
    }

    return result;
}

void Cell::move(Direction direction)
{
    auto next = nextPosition(direction);
    x = next.first;
    y = next.second;
    position->x = next.first  * width();
    position->y = next.second * height() + yOffset;
}
