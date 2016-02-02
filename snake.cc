#include <memory>
#include <stdexcept>
#include "snake.hh"
#include "cell.hh"

namespace {
    const Direction UP    = Direction::UP;
    const Direction DOWN  = Direction::DOWN;
    const Direction LEFT  = Direction::LEFT;
    const Direction RIGHT = Direction::RIGHT;
}

Snake::Snake(SDL_Surface *surface, unsigned x, unsigned y, Direction direction):
    direction(direction), needsToGrow(false), surface(surface)
{
    auto color = getColor();
    body.push_back(std::unique_ptr<Cell>{new Cell(surface, x, y, color)});
}

Snake::Snake(SDL_Surface *surface, unsigned x, unsigned y, Direction direction,
             unsigned screenWidth, unsigned screenHeight, unsigned yOffset,
             decltype(body.size()) initialLength):
    direction(direction), needsToGrow(false), surface(surface)
{
    auto color = getColor();
    if (initialLength == 0) {
        throw std::invalid_argument("Initial snake length cannot be zero");
    } else if (x * Cell::width()  > screenWidth ||
               y * Cell::height() > screenHeight) {
        throw std::invalid_argument("Initial snake coordinates not on screen");
    } else {
        body.push_back(std::unique_ptr<Cell>{new Cell(surface, x, y, color)});
        growToInitialLength(initialLength, screenWidth, screenHeight, yOffset);
    }
}

void Snake::growToInitialLength(decltype(body.size()) initialLength,
                                unsigned screenWidth, unsigned screenHeight,
                                unsigned yOffset)
{
    unsigned x = head().xPosition() * Cell::width();
    unsigned y = head().yPosition() * Cell::height() + yOffset;
    Direction primaryDirection = direction;
    Direction secondaryDirection;
    switch (direction) {
    case UP: case DOWN:
        secondaryDirection = (screenWidth - x) > x ? RIGHT : LEFT;
        break;
    case LEFT: case RIGHT:
        secondaryDirection = (screenHeight - y) > y ? DOWN : UP;
        break;
    }
    while (--initialLength) {
        auto next = nextPosition();
        unsigned x = next.first, y = next.second;
        // Give a two cell border to prevent immediate deaths
        if (x < Cell::width()  * 2           ||
            y < Cell::height() * 2 + yOffset ||
            x >= screenWidth  - Cell::width()  * 2 ||
            y >= screenHeight - Cell::height() * 2) {
            if (direction == secondaryDirection) {
                throw std::runtime_error("Initial snake length too large");
            } else {
                direction = secondaryDirection;
            }
        } else {
            grow();
            move();
            if (direction == secondaryDirection) {
                primaryDirection = invert(primaryDirection);
                direction = primaryDirection;
            }
        }
    }
}
