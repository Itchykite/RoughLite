#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <vector>
#include <random>
#include "Player.hpp"
#include "Enemy.hpp"

class EnemyManager
{
public:
    EnemyManager(Player* player, SDL_Renderer* renderer);

    void AddEnemy();
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);

private:
    std::vector<std::unique_ptr<Enemy>> enemies;
    Player* player;
    SDL_Renderer* renderer;

    float GetRandomFloat(float min, float max);
};