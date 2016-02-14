#ifndef SNAKE_HH
#define SNAKE_HH

#include <algorithm>
#include <functional>
#include <memory>
#include <utility>
#include <vector>
#include <SDL.h>
#include "cell.hh"
#include "direction.hh"

class Snake {
private:
    std::vector<std::unique_ptr<Cell>> body;
public:
    Snake(SDL_Surface *surface, unsigned x, unsigned y, Direction direction);
    Snake(SDL_Surface *surface, unsigned x, unsigned y, Direction direction,
          unsigned screenWidth, unsigned screenHeight,
          decltype(body.size()) initialLength);
    // Set the snake's next movement to be in the specified direction
    void changeDirection(Direction direction);
    bool collidesWith(const Cell& cell) const;
    bool collidesWith(const std::pair<unsigned, unsigned>& coords) const;
    void draw() const;
    // Cause the snake to grow by one cell after the next call to
    // move()
    void grow();
    void move();
    std::pair<int, int> nextPosition() const;
    const Cell& head() const;
    void growToInitialLength(decltype(body.size()) initialLength,
                             unsigned screenWidth, unsigned screenHeight);
    bool willRunIntoSelf() const;
private:
    Direction direction;
    bool needsToGrow;
    SDL_Surface *surface;
    Uint32 getColor() const;
};

inline void Snake::changeDirection(Direction direction)
{
    this->direction = direction;
}

inline bool Snake::collidesWith(const Cell& cell) const
{
    auto equal = [&cell](const std::unique_ptr<Cell>& ptr)
        { return cell == *ptr; };
    return std::find_if(body.begin(), body.end(), equal) != body.end();
}

inline bool Snake::collidesWith(const std::pair<unsigned, unsigned>& pos) const
{
    auto equal = [&pos](const std::unique_ptr<Cell>& ptr)
        { return *ptr == pos; };
    return std::find_if(body.begin(), body.end(), equal) != body.end();
}

inline void Snake::draw() const
{
    std::for_each(body.begin(), body.end(), std::mem_fn(&Cell::draw));
}

inline Uint32 Snake::getColor() const
{
    return SDL_MapRGB(surface->format, 0, 127, 127);
}

inline void Snake::grow()
{
    needsToGrow = true;
}

inline const Cell& Snake::head() const
{
    return *body.back();
}

inline void Snake::move()
{
    auto next = nextPosition();
    unsigned x = next.first, y = next.second;
    body.push_back(std::unique_ptr<Cell>{new Cell(surface, x, y, getColor())});
    if (needsToGrow) needsToGrow = false;
    else body.erase(body.begin());
}

inline std::pair<int, int> Snake::nextPosition() const
{
    return head().nextPosition(direction);
}

inline bool Snake::willRunIntoSelf() const
{
    auto next = nextPosition();
    auto equal = [&next](const std::unique_ptr<Cell>& cell)
        { return *cell == next; };
    return std::find_if(++body.begin(), body.end(), equal) != body.end();
}

#endif
