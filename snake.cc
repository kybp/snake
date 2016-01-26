#include "snake.hh"
#include "cell.hh"

Snake::Snake(SDL_Surface *surface, int x, int y, Direction direction)
{
    this->needsToGrow = false;
    this->surface = surface;
    this->direction = direction;
    auto color = SDL_MapRGB(surface->format, 0, 127, 127);
    body = std::vector<Cell>(1, Cell(surface, x, y, color));
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

bool Snake::move()
{
    Cell next(body.back());
    next.move(direction);
    if (collidesWith(next)) {
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
