#include <cstdlib>
#include "cell.hh"
#include "direction.hh"

using Direction::UP;
using Direction::DOWN;
using Direction::LEFT;
using Direction::RIGHT;

Cell::Cell(SDL_Surface *surface, int x, int y)
{
    this->surface = surface;
    SDL_Rect *position = new SDL_Rect;
    position->x = x;
    position->y = y;
    position->w = width;
    position->h = height;
    this->position = position;
    this->color = SDL_MapRGB(surface->format, 0, 127, 127);
}

void Cell::move(Direction direction)
{
    switch (direction) {
    case UP:
        position->y -= height;
        break;
    case DOWN:
        position->y += height;
        break;
    case LEFT:
        position->x -= width;
        break;
    case RIGHT:
        position->x += width;
        break;
    }
}
