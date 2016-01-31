#ifndef CELL_HH
#define CELL_HH

#include <memory>
#include <utility>
#include <SDL.h>
#include "direction.hh"

class Cell {
public:
    Cell() = default;
    Cell(SDL_Surface *surface, unsigned x, unsigned y, Uint32 color);
    // Draw the cell to its SDL_Surface
    void draw() const;
    // Move the cell one square of the game grid
    void move(Direction direction);
    std::pair<unsigned, unsigned> nextPosition(Direction direction) const;
    // Return the cell's x position on the game grid
    unsigned xPosition() const;
    // Return the cell's y position on the game grid
    unsigned yPosition() const;
    // Return true if both cells have the same grid coordinates
    bool operator==(const Cell& rhs) const;
    bool operator!=(const Cell& rhs) const;
    bool operator==(const std::pair<unsigned, unsigned>& rhs) const;
    bool operator!=(const std::pair<unsigned, unsigned>& rhs) const;
    // Return the current height of a cell
    static unsigned height();
    // Set the height of a cell
    static void setHeight(unsigned height);
    // Return the current width of a cell
    static unsigned width();
    // Set the width of a cell
    static void setWidth(unsigned width);
    // Set the height of the margin to leave at the top of the screen
    // before the game grid begins
    static void setYOffset(unsigned offset);
private:
    unsigned x, y;
    std::unique_ptr<SDL_Rect> position;
    SDL_Surface *surface;
    Uint32 color;
    static unsigned w, h, yOffset;
};

inline void Cell::draw() const
{
    SDL_FillRect(surface, position.get(), color);
}

inline unsigned Cell::height()
{
    return h;
}

inline void Cell::setHeight(unsigned height)
{
    h = height;
}

inline unsigned Cell::width()
{
    return w;
}

inline void Cell::setWidth(unsigned width)
{
    w = width;
}

inline unsigned Cell::xPosition() const
{
    return x;
}

inline unsigned Cell::yPosition() const
{
    return y;
}

inline bool Cell::operator==(const Cell& rhs) const
{
    return (xPosition() == rhs.xPosition() &&
            yPosition() == rhs.yPosition());
}

inline bool Cell::operator!=(const Cell& rhs) const
{
    return !(*this == rhs);
}

inline bool Cell::operator==(const std::pair<unsigned, unsigned>& rhs) const
{
    std::pair<unsigned, unsigned> lhs{ xPosition(), yPosition() };
    return lhs == rhs;
}

inline bool Cell::operator!=(const std::pair<unsigned, unsigned>& rhs) const
{
    return !(*this == rhs);
}

inline void Cell::setYOffset(unsigned offset)
{
    yOffset = offset;
}

#endif
