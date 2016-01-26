#ifndef CELL_HH
#define CELL_HH

#include <SDL.h>
#include "direction.hh"

class Cell {
public:
    Cell(SDL_Surface *surface, int x, int y, Uint32 color);
    Cell(const Cell& rhs) :
        Cell(rhs.surface, rhs.xPosition(), rhs.yPosition(), rhs.color)
        {};
    void draw();
    void move(Direction direction);
    int xPosition() const;
    int yPosition() const;
    static const int width  = 10;
    static const int height = 10;
private:
    SDL_Rect *position;
    SDL_Surface *surface;
    Uint32 color;
};

inline void Cell::draw()
{
    SDL_FillRect(surface, position, color);
}

inline int Cell::xPosition() const
{
    return position->x;
}

inline int Cell::yPosition() const
{
    return position->y;
}

#endif
