#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>
#include "cell.hh"
#include "layout.hh"

Layout::Layout(int heightInCells, int widthInCells):
    heightInPixels(heightInCells * Cell::height()),
    widthInPixels(widthInCells   * Cell::width())
{}

Layout::Layout(std::string filename, SDL_Surface *surface)
{
    std::ifstream file(filename);
    auto color = SDL_MapRGB(surface->format, 127, 0, 0);

    if (file) {
        std::string line;
        for (heightInPixels = 0; getline(file, line);
             heightInPixels += Cell::height()) {
            auto length = line.length() * Cell::width();
            if (length > widthInPixels) widthInPixels = length;

            int x = 0;
            for (auto c = line.begin(); c != line.end();
                 x += Cell::width(), ++c) {
                switch (*c) {
                case '#': {
                    auto cell = new Cell(surface, x, heightInPixels, color);
                    layout.push_back(cell);
                } break;
                case ' ':       // ignore empty space
                    break;
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
