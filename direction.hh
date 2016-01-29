#ifndef DIRECTION_HH
#define DIRECTION_HH

enum class Direction { UP, DOWN, LEFT, RIGHT };
Direction directionFromChar(char c);
Direction invert(Direction direction);

#endif
