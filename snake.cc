#include "snake.hh"
#include "cell.hh"

using Direction::UP;
using Direction::DOWN;
using Direction::LEFT;
using Direction::RIGHT;

Snake::Snake(SDL_Surface *surface, int cellX, int cellY, Direction direction):
    direction(direction), needsToGrow(false), surface(surface)
{
    auto color = getColor();
    body = std::vector<Cell>(1, Cell(surface, cellX, cellY, color));
}

Snake::Snake(SDL_Surface *surface, int cellX, int cellY,
             Direction direction, int screenWidth, int screenHeight,
             decltype(body.size()) initialLength):
    direction(direction), needsToGrow(false), surface(surface)
{
    auto color = getColor();
    if (initialLength == 0) {
        throw std::invalid_argument("Initial snake length cannot be zero");
    } else if (cellX * Cell::width()  > screenWidth ||
               cellY * Cell::height() > screenHeight) {
        throw std::invalid_argument("Initial snake coordinates not on screen");
    } else {
        body = std::vector<Cell>(1, Cell(surface, cellX, cellY, color));
        growToInitialLength(initialLength, screenWidth, screenHeight);
    }
}

void Snake::changeDirection(Direction direction)
{
    this->direction = direction;
}

bool Snake::collidesWith(const Cell& cell) const
{
    for (Cell other : body) {
        if (cell.xPositionInPixels() == other.xPositionInPixels() &&
            cell.yPositionInPixels() == other.yPositionInPixels()) {
            return true;
        }
    }
    return false;
}

void Snake::draw() const
{
    for (Cell cell : body) {
        cell.draw();
    }
}

void Snake::growToInitialLength(decltype(body.size()) initialLength,
                                int screenWidth, int screenHeight)
{
    int x = xPosition(), y = yPosition();
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
        Cell next(body.back());
        next.move(direction);
        x = next.xPositionInPixels(), y = next.yPositionInPixels();
        // Give a two cell border to prevent immediate deaths
        if (x < Cell::width()  * 2 || x >= screenWidth  - Cell::width()  * 2 ||
            y < Cell::height() * 2 || y >= screenHeight - Cell::height() * 2) {
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

bool Snake::move()
{
    const Cell first = *body.cbegin();
    Cell next(body.back());
    next.move(direction);
    if (collidesWith(next) && 
        !(first.xPositionInPixels() == next.xPositionInPixels() &&
          first.yPositionInPixels() == next.yPositionInPixels())) {
        return true;
    } else {
        body.push_back(next);
        if (needsToGrow) {
            needsToGrow = false;
        } else {
            body.erase(body.begin());
        }
        return false;
    }
}
