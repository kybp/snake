#include <stdexcept>
#include "snake.hh"
#include "cell.hh"

static const Direction UP    = Direction::UP;
static const Direction DOWN  = Direction::DOWN;
static const Direction LEFT  = Direction::LEFT;
static const Direction RIGHT = Direction::RIGHT;

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

bool Snake::collidesWithCells(const std::pair<int, int>& coordinates) const
{
    for (Cell cell : body) {
        if (cell.xPositionInCells() == coordinates.first &&
            cell.yPositionInCells() == coordinates.second) {
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
    int x = xPositionInPixels(), y = yPositionInPixels();
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
    Cell last = body.front();
    Cell next(body.back());
    next.move(direction);
    int x = next.xPositionInCells(), y = next.yPositionInCells();
    std::pair<int, int> coordinates(x, y);
    if (collidesWithCells(coordinates) && 
        !(x == last.xPositionInCells() && y == last.yPositionInCells())) {
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
