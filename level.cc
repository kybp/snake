#include <algorithm>
#include <memory>
#include <vector>
#include <SDL.h>
#include "cell.hh"
#include "level.hh"

Level::Level(SDL_Surface *surface, unsigned width, unsigned height,
             unsigned *score)
    : score(score), surface(surface), alive(true), winnable(false), won(false),
      width(width), height(height),
      snake(std::unique_ptr<Snake>
            { new Snake(surface, height / 2, width / 2, Direction::LEFT) })
{
    generateRandomFood();
}

void Level::draw()
{
    SDL_FillRect(surface, nullptr, 0);
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
        for (auto it = food.begin(); it != food.end(); ++it) {
            if (snake->head() == **it) {
                snake->grow();
                ++*score;
                food.erase(it);
                if (!winnable) {
                    generateRandomFood();
                } else if (food.empty()) {
                    alive = false;
                    won   = true;
                }
            }
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
