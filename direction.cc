#include "direction.hh"

static const Direction UP    = Direction::UP;
static const Direction DOWN  = Direction::DOWN;
static const Direction LEFT  = Direction::LEFT;
static const Direction RIGHT = Direction::RIGHT;

Direction invert(Direction direction)
{
    switch (direction) {
    case UP:    return DOWN;
    case DOWN:  return UP;
    case LEFT:  return RIGHT;
    case RIGHT: return LEFT;
    }
}
