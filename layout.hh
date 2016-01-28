#ifndef LAYOUT_HH
#define LAYOUT_HH

#include <vector>
#include <SDL.h>
#include "cell.hh"

class Layout {
public:
    Layout(int heightInCells, int widthInCells);
    Layout(std::string filename, SDL_Surface *surface);
    bool contains(const Cell cell) const;
    void draw() const;
    unsigned getHeightInCells() const;
    unsigned getWidthInCells() const;
    unsigned getHeightInPixels() const;
    unsigned getWidthInPixels() const;
private:
    std::vector<Cell *> layout;
    unsigned heightInPixels;
    unsigned widthInPixels;
};

inline unsigned Layout::getHeightInCells() const
{
    return getHeightInPixels() / Cell::height;
}

inline unsigned Layout::getWidthInCells() const
{
    return getWidthInPixels() / Cell::width;
}

inline unsigned Layout::getHeightInPixels() const
{
    return heightInPixels;
}

inline unsigned Layout::getWidthInPixels() const
{
    return widthInPixels;
}

#endif
