#include <cctype>
#include <stdexcept>
#include "direction.hh"

static const Direction UP    = Direction::UP;
static const Direction DOWN  = Direction::DOWN;
static const Direction LEFT  = Direction::LEFT;
static const Direction RIGHT = Direction::RIGHT;

Direction directionFromChar(char c)
{
    switch (std::tolower(c)) {
    case 'u': return UP;
    case 'd': return DOWN;
    case 'l': return LEFT;
    case 'r': return RIGHT;
    default:
        throw std::invalid_argument("Direction char not one of U, D, L, or R");
    }
}

Direction invert(Direction direction)
{
    Direction result;

    switch (direction) {
    case UP:    result = DOWN;  break;
    case DOWN:  result = UP;    break;
    case LEFT:  result = RIGHT; break;
    case RIGHT: result = LEFT;  break;
    }

    return result;
}
