#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>
#include "cell.hh"
#include "layout.hh"

Layout::Layout(int heightInCells, int widthInCells):
    heightInCells(heightInCells), widthInCells(widthInCells)
{}

Layout::Layout(std::string filename, SDL_Surface *surface)
{
    std::ifstream file(filename);
    auto color = SDL_MapRGB(surface->format, 127, 0, 0);

    if (file) {
        std::string line;
        for (heightInCells = 0; getline(file, line); ++heightInCells) {
            auto length = line.length();
            if (length > widthInCells) widthInCells = length;

            for (unsigned x = 0; x < line.length(); ++x) {
                switch (line[x]) {
                case '#': {
                    auto cell = new Cell(surface, x, heightInCells, color);
                    layout.push_back(cell);
                } break;
                case 'S': {
                    startingCellX = x;
                    startingCellY = heightInCells;
                } break;
                case ' ': break; // ignore empty space
                default:
                    throw std::runtime_error("Invalid character in layout");
                }
            }
        }
    }
}

bool Layout::contains(const Cell cell) const
{
    for (auto us : layout) {
        if (us->xPositionInPixels() == cell.xPositionInPixels() &&
            us->yPositionInPixels() == cell.yPositionInPixels()) {
            return true;
        }
    }
    return false;
}

void Layout::draw() const
{
    for (auto cell : layout) {
        cell->draw();
    }
}

void Layout::updatePosition()
{
    for (auto cell : layout) {
        cell->updatePosition();
    }
}
