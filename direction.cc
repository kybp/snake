#include <cctype>
#include <stdexcept>
#include "direction.hh"

namespace {
    const Direction UP    = Direction::UP;
    const Direction DOWN  = Direction::DOWN;
    const Direction LEFT  = Direction::LEFT;
    const Direction RIGHT = Direction::RIGHT;
}

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
