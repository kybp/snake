#include <algorithm>
#include <memory>
#include <vector>
#include <SDL.h>
#include "cell.hh"
#include "food.hh"
#include "level.hh"

Level::Level(SDL_Surface *surface, unsigned height, unsigned width, int *score)
    : score(score), surface(surface), alive(true),
      height(height), width(width),
      snake(std::unique_ptr<Snake>
            { new Snake(surface, height / 2, width / 2, Direction::LEFT) })
{
    generateRandomFood();
}

void Level::draw()
{
    snake->draw();
    for (const auto& cell : food)  cell->draw();
    for (const auto& cell : walls) cell->draw();
}

void Level::generateRandomFood()
{
    int x = rand() % width;
    int y = rand() % height;
    food.push_back(std::unique_ptr<Food>(new Food(surface, x, y)));
}

void Level::update()
{
    if (snake->willRunIntoSelf() || snakeWillRunIntoWall()) {
        alive = false;
    } else {
        snake->move();
        const Cell& head = snake->head();
        auto equal = [&head](const std::unique_ptr<Food>& ptr)
            { return head == *ptr; };
        if (std::find_if(food.begin(), food.end(), equal) != food.end()) {
            snake->grow();
            ++*score;
        }
    }
}

bool Level::snakeWillRunIntoWall() const
{
    auto next = snake->nextPosition();
    int x = next.first, y = next.second;
    if (x < 0 || y < 0 || (unsigned)x >= width || (unsigned)y >= height)
        return true;
    for (const auto& cell : walls) {
        if (*cell == next) return true;
    }
    return false;
}
