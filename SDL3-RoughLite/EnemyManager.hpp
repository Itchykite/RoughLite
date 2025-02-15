#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <vector>
#include <random>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Settings.hpp"

class EnemyManager
{
public:
    EnemyManager(Player* player, Map* map, Camera* camera, SDL_Renderer* renderer);

    void AddEnemy();
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);

private:
    bool CheckCollision(const SDL_FRect& rect1, const SDL_FRect& rect2) const;
    std::vector<std::unique_ptr<Enemy>> enemies;
    Player* player;
    Map* map;
	Camera* camera;

    SDL_Renderer* renderer;

    float GetRandomFloat(float min, float max);
};