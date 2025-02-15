#include "EnemyManager.hpp"

EnemyManager::EnemyManager(Player* player, SDL_Renderer* renderer)
    : player(player), renderer(renderer) {
}

void EnemyManager::AddEnemy()
{
    float startX = GetRandomFloat(0.0f, 2000.0f);
    float startY = GetRandomFloat(0.0f, 2000.0f);

    auto enemy = std::make_unique<Enemy>(player, renderer);
    enemy->SetPosition(startX, startY);
    enemies.push_back(std::move(enemy));
}

void EnemyManager::Update(float deltaTime)
{
    for (auto& enemy : enemies)
    {
        enemy->Update(deltaTime);
    }
}

void EnemyManager::Render(SDL_Renderer* renderer)
{
    for (auto& enemy : enemies)
    {
        enemy->Render(renderer);
    }
}

float EnemyManager::GetRandomFloat(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}