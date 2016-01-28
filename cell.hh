#ifndef CELL_HH
#define CELL_HH

#include <SDL.h>
#include "direction.hh"

class Cell {
public:
    Cell(SDL_Surface *surface, int x, int y, Uint32 color);
    void draw() const;
    void move(Direction direction);
    void setSurface(SDL_Surface *surface);
    int xPosition() const;
    int yPosition() const;
    static const int width  = 40;
    static const int height = 40;
private:
    SDL_Rect position;
    SDL_Surface *surface;
    Uint32 color;
};

inline void Cell::draw() const
{
    SDL_FillRect(surface, &position, color);
}

inline void Cell::setSurface(SDL_Surface *surface)
{
    this->surface = surface;
}

inline int Cell::xPosition() const
{
    return position.x;
}

inline int Cell::yPosition() const
{
    return position.y;
}

#endif
