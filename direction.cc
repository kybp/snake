#include "direction.hh"

static const Direction UP    = Direction::UP;
static const Direction DOWN  = Direction::DOWN;
static const Direction LEFT  = Direction::LEFT;
static const Direction RIGHT = Direction::RIGHT;

Direction invert(Direction direction)
{
    Direction result;

    switch (direction) {
    case UP:    result = DOWN;
    case DOWN:  result = UP;
    case LEFT:  result = RIGHT;
    case RIGHT: result = LEFT;
    }

    return result;
}
