#include "snake.hh"
#include "cell.hh"

using Direction::UP;
using Direction::DOWN;
using Direction::LEFT;
using Direction::RIGHT;

Snake::Snake(SDL_Surface *surface, int x, int y, Direction direction):
    direction(direction), needsToGrow(false), surface(surface)
{
    auto color = getColor();
    body = std::vector<Cell>(1, Cell(surface, x, y, color));
}

Snake::Snake(SDL_Surface *surface, int x, int y, Direction direction,
             int screenWidth, int screenHeight,
             decltype(body.size()) initialLength):
    direction(direction), needsToGrow(false), surface(surface)
{
    auto color = getColor();
    if (initialLength == 0) {
        throw std::invalid_argument("Initial snake length cannot be zero");
    } else if (x > screenWidth || y > screenHeight) {
        throw std::invalid_argument("Initial snake coordinates not on screen");
    } else {
        body = std::vector<Cell>(1, Cell(surface, x, y, color));
        growToInitialLength(initialLength, screenWidth, screenHeight);
    }
}

void Snake::changeDirection(Direction direction)
{
    this->direction = direction;
}

bool Snake::collidesWith(const Cell& cell)
{
    for (Cell other : body) {
        if (cell.xPosition() == other.xPosition() &&
            cell.yPosition() == other.yPosition()) {
            return true;
        }
    }
    return false;
}

void Snake::draw()
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
        x = next.xPosition(), y = next.yPosition();
        // Give a two cell border to prevent immediate deaths
        if (x < Cell::width  * 2 || x >= screenWidth  - Cell::width * 2 ||
            y < Cell::height * 2 || y >= screenHeight - Cell::height * 2) {
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
    Cell first = *body.begin();
    Cell next(body.back());
    next.move(direction);
    if (collidesWith(next) && 
        !(first.xPosition() == next.xPosition() &&
          first.yPosition() == next.yPosition())) {
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
