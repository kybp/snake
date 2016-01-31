#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <SDL.h>
#include "cell.hh"
#include "food.hh"
#include "level.hh"

Layout::Layout(SDL_Surface *surface, int offsetInCells,
               int heightInCells, int widthInCells)
    : winnable(false), surface(surface),
      heightInCells(heightInCells + offsetInCells), widthInCells(widthInCells)
{}

Layout::Layout(SDL_Surface *surface, std::string filename,
               int offsetInCells)
    : winnable(false), startingDirection(Direction::RIGHT),
      surface(surface), widthInCells(0)
{
    std::ifstream file(filename);
    auto color = SDL_MapRGB(surface->format, 127, 0, 0);

    if (file) {
        std::string line;
        for (heightInCells = offsetInCells; getline(file, line);
             ++heightInCells) {
            auto length = line.length();
            if (length > widthInCells) widthInCells = length;

            for (unsigned x = 0; x < line.length(); ++x) {
                switch (line[x]) {
                case '#': {
                    auto cell = std::make_shared<Cell>(
                        surface, x, heightInCells, color);
                    layout.push_back(cell);
                } break;
                case 'U': case 'D': case 'L': case 'R': {
                    startingCellX = x;
                    startingCellY = heightInCells;
                    startingDirection = directionFromChar(line[x]);
                } break;
                case 'F': {
                    winnable = true;
                    add_food_at(x, heightInCells);
                } break;
                case ' ': break; // ignore empty space
                default:
                    throw std::runtime_error("Invalid character in layout");
                }
            }
        }
    } else {
        std::cerr << "Can't open file: " << filename << std::endl;
        exit(1);
    }
}

bool Layout::containsCellCoordinates(int x, int y) const
{
    for (auto cell : layout) {
        if (cell->xPositionInCells() == x && cell->yPositionInCells() == y) {
            return true;
        }
    }
    return false;
}

bool Layout::containsCellCoordinates(const std::pair<int, int>& coordinates)
    const
{
    return containsCellCoordinates(coordinates.first, coordinates.second);
}

void Layout::draw() const
{
    for (auto cell : layout) cell->draw();
    for (auto cell : food)   cell->draw();
}

bool Layout::eat_food_at(int pixelX, int pixelY)
{
    auto samePosition = [pixelX, pixelY](std::shared_ptr<Food> cell) {
        return (cell->xPositionInPixels() == pixelX &&
                cell->yPositionInPixels() == pixelY);
    };

    decltype(food.begin()) it;
    if ((it = std::find_if(food.begin(), food.end(), samePosition)) !=
        food.end()) {
        food.erase(it);
        return true;
    }
    return false;
}

void Layout::updatePosition()
{
    for (auto cell : layout) cell->updatePosition();
    for (auto cell : food)   cell->updatePosition();
}
