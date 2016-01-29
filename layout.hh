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
    int getStartingXCell() const;
    int getStartingYCell() const;
    void updatePosition();
private:
    std::vector<Cell *> layout;
    int startingCellX;
    int startingCellY;
    unsigned heightInCells;
    unsigned widthInCells;
};

inline unsigned Layout::getHeightInCells() const
{
    return heightInCells;
}

inline unsigned Layout::getWidthInCells() const
{
    return widthInCells;
}

inline unsigned Layout::getHeightInPixels() const
{
    return getHeightInCells() * Cell::height();
}

inline unsigned Layout::getWidthInPixels() const
{
    return getWidthInCells() * Cell::width();
}

inline int Layout::getStartingXCell() const
{
    return startingCellX;
}

inline int Layout::getStartingYCell() const
{
    return startingCellY;
}

#endif
