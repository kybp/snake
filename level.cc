#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <SDL.h>
#include "cell.hh"
#include "level.hh"

Level::Level(SDL_Surface *surface, unsigned width, unsigned height,
             unsigned *score, unsigned *highScore, unsigned initialLength)
    : score(score), highScore(highScore), surface(surface),
      alive(true), winnable(false), won(false),
      width(width), height(height),
      startingX(width / 2), startingY(height / 2),
      startingDirection(Direction::LEFT),
      snake(std::unique_ptr<Snake>
            { new Snake(surface, startingX, startingY, startingDirection,
                        width * Cell::width(),
                        height * Cell::height() + Cell::getYOffset(),
                        initialLength)})
{
    generateRandomFood();
}

Level::Level(SDL_Surface *surface, unsigned screenWidth, unsigned screenHeight,
             const char *filename, unsigned *score, unsigned *highScore)
    : score(score), highScore(highScore), surface(surface),
      alive(true), winnable(false), won(false),
      width(0), startingX(0), startingY(0), startingDirection(Direction::RIGHT)
{
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Can't open file: " << filename << std::endl;
        exit(1);
    }

    std::string line;
    std::vector<std::pair<unsigned, unsigned>> foodPoints;
    std::vector<std::pair<unsigned, unsigned>> wallPoints;

    for (height = 0; getline(file, line); ++height) {
        auto length = line.length();
        if (length > width) width = length;

        for (unsigned x = 0; x < line.length(); ++x) {
            switch (line[x]) {
            case '#': {
                auto coords = std::pair<unsigned, unsigned>{x, height};
                wallPoints.push_back(coords);
            } break;
            case 'U': case 'D': case 'L': case 'R': {
                startingX = x;
                startingY = height;
                startingDirection = directionFromChar(line[x]);
            } break;
            case 'F': {
                winnable = true;
                auto coords = std::pair<unsigned, unsigned>{x, height};
                foodPoints.push_back(coords);
            } break;
            case ' ': break;    // ignore empty spaces
            default:
                throw std::runtime_error("Invalid charactor in layout");
            }
        }
    }

    Cell::setWidth(screenWidth / width);
    Cell::setHeight((screenHeight - Cell::getYOffset()) / height);

    snake = std::unique_ptr<Snake>(
        new Snake(surface, startingX, startingY, startingDirection));

    for (const auto& coords : foodPoints) {
        food.push_back(std::unique_ptr<Food>(
                           new Food(surface, coords.first, coords.second)));
    }

    for (const auto& coords : wallPoints) {
        walls.push_back(std::unique_ptr<Wall>(
                            new Wall(surface, coords.first, coords.second)));
    }

    if (!winnable) generateRandomFood();
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
    static std::default_random_engine e;
    static std::uniform_int_distribution<unsigned> w(0, width  - 1);
    static std::uniform_int_distribution<unsigned> h(0, height - 1);
    std::pair<unsigned, unsigned> nextFood;

    do nextFood = { w(e), h(e) };
    while (invalidFoodPosition(nextFood));

    food.push_back(std::unique_ptr<Food>(new Food(surface, nextFood)));
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

void Level::reset()
{
    alive = true;
    *score = 0;
    snake = std::unique_ptr<Snake>(
        new Snake(surface, startingX, startingY, startingDirection));
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
                if (*score > *highScore) *highScore = *score;
                food.erase(it);
                if (!winnable) {
                    generateRandomFood();
                } else if (food.empty()) {
                    alive = false;
                    won   = true;
                }
                // Needed because we erased it, but you can only eat
                // one food cell in one move anyway
                break;
            }
        }
    }
}

bool
Level::invalidFoodPosition(const std::pair<unsigned, unsigned>& coords) const
{
    if (snake->collidesWith(coords)) return true;
    for (const auto& cell : food)  if (*cell == coords) return true;
    for (const auto& cell : walls) if (*cell == coords) return true;
    return false;
}
