#ifndef FOOD_H
#define FOOD_H

#include <SDL.h>
#include "cell.hh"

class Food : public Cell {
public:
    Food(SDL_Surface *surface, int x, int y)
        : Cell(surface, x, y, SDL_MapRGB(surface->format, 0, 127, 0))
        {}
};

#endif
