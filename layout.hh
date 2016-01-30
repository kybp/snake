#ifndef LAYOUT_HH
#define LAYOUT_HH

#include <utility>
#include <vector>
#include "cell.hh"
#include "direction.hh"

struct SDL_Surface;
class Food;

class Layout {
public:
    Layout(SDL_Surface *surface, int heightInCells, int widthInCells);
    Layout(SDL_Surface *surface, std::string filename);
    void add_food_at(int x, int y);
    void add_food_at(const std::pair<int, int>& coordinates);
    bool containsCellCoordinates(int x, int y) const;
    bool containsCellCoordinates(const std::pair<int, int>& coordinates) const;
    void draw() const;
    // Return value indicates whether there was food at that position
    bool eat_food_at(int pixelX, int pixelY);
    unsigned getHeightInCells() const;
    unsigned getWidthInCells() const;
    unsigned getHeightInPixels() const;
    unsigned getWidthInPixels() const;
    int getStartingXCell() const;
    int getStartingYCell() const;
    Direction getStartingDirection() const;
    bool isWinnable() const;
    bool noFoodLeft() const;
    void updatePosition();
private:
    bool winnable;
    std::vector<Food *> food;
    std::vector<Cell *> layout;
    int startingCellX;
    int startingCellY;
    Direction startingDirection;
    SDL_Surface *surface;
    unsigned heightInCells;
    unsigned widthInCells;
};

inline void Layout::add_food_at(int x, int y)
{
    food.push_back(new Food(surface, x, y));
}

inline void Layout::add_food_at(const std::pair<int, int>& coordinates)
{
    add_food_at(coordinates.first, coordinates.second);
}

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

inline Direction Layout::getStartingDirection() const
{
    return startingDirection;
}

inline int Layout::getStartingXCell() const
{
    return startingCellX;
}

inline int Layout::getStartingYCell() const
{
    return startingCellY;
}

inline bool Layout::noFoodLeft() const
{
    return food.empty();
}

inline bool Layout::isWinnable() const
{
    return winnable;
}

#endif
