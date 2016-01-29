#ifndef CELL_HH
#define CELL_HH

#include <SDL.h>
#include "direction.hh"

class Cell {
public:
    Cell(SDL_Surface *surface, int x, int y, Uint32 color);
    void draw() const;
    void move(Direction direction);
    void updatePosition();
    int xPositionInPixels() const;
    int yPositionInPixels() const;
    int xPositionInCells() const;
    int yPositionInCells() const;
    static int height();
    static int width();
    static void setHeight(int height);
    static void setWidth(int width);
private:
    static int h, w;
    int cellX, cellY;
    SDL_Rect position;
    SDL_Surface *surface;
    Uint32 color;
};

inline void Cell::draw() const
{
    SDL_FillRect(surface, &position, color);
}

inline void Cell::setHeight(int height)
{
    h = height;
}

inline void Cell::setWidth(int width)
{
    w = width;
}

inline void Cell::updatePosition()
{
    position.x = xPositionInPixels();
    position.y = yPositionInPixels();
    position.w = width();
    position.h = height();
}

inline int Cell::xPositionInCells() const
{
    return cellX;
}

inline int Cell::yPositionInCells() const
{
    return cellY;
}

inline int Cell::xPositionInPixels() const
{
    return xPositionInCells() * width();
}

inline int Cell::yPositionInPixels() const
{
    return yPositionInCells() * height();
}

#endif
