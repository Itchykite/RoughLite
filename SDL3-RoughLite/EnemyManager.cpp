#define _USE_MATH_DEFINES

#include "EnemyManager.hpp"

EnemyManager::EnemyManager(Player* player, Map* map, Camera* camera, SDL_Renderer* renderer)
    : player(player), map(map), camera(camera), renderer(renderer)
{
}

void EnemyManager::AddEnemy()
{
    float enemyX, enemyY;
    bool validPosition = false;

    while (!validPosition)
    {
        enemyX = GetRandomFloat(0, mapWidth - Enemy::enemyW);
        enemyY = GetRandomFloat(0, mapHeight - Enemy::enemyH);

        validPosition = true;
        SDL_FRect newEnemyRect = { enemyX, enemyY, Enemy::enemyW, Enemy::enemyH };

        for (const auto& enemy : enemies)
        {
            if (CheckCollision(newEnemyRect, enemy->GetCollisionRect()))
            {
                validPosition = false;
                break;
            }
        }
    }

    auto enemy = std::make_unique<Enemy>(player, map, camera, renderer);
    enemy->SetPosition(enemyX, enemyY);
    enemies.push_back(std::move(enemy));
}

void EnemyManager::AddRangeRover()
{
    float angleStep = 360.0f / RangeRover::numOfRangeRovers;
    float radius = 200.0f;

    for (int i = 0; i < RangeRover::numOfRangeRovers; ++i)
    {
        float angle = i * angleStep;
        float enemyX = player->GetX() + radius * std::cos(angle * M_PI / 180.0f);
        float enemyY = player->GetY() + radius * std::sin(angle * M_PI / 180.0f);

        auto rangeRover = std::make_unique<RangeRover>(player, map, camera, renderer);
        rangeRover->SetPosition(enemyX, enemyY);
        rangeRovers.push_back(std::move(rangeRover));
    }
}

void EnemyManager::Update(float deltaTime)
{
    for (auto& enemy : enemies)
    {
        enemy->Update(deltaTime);
    }

    for (size_t i = 0; i < enemies.size(); ++i)
    {
        for (size_t j = i + 1; j < enemies.size(); ++j)
        {
            auto& enemy1 = enemies[i];
            auto& enemy2 = enemies[j];

            SDL_FRect rect1 = enemy1->GetCollisionRect();
            SDL_FRect rect2 = enemy2->GetCollisionRect();

            if (CheckCollision(rect1, rect2))
            {
                float dx = (rect1.x + rect1.w / 2) - (rect2.x + rect2.w / 2);
                float dy = (rect1.y + rect1.h / 2) - (rect2.y + rect2.h / 2);
                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance == 0) distance = 0.001f;

                float overlap = (rect1.w / 2 + rect2.w / 2) - distance;
                float pushX = (dx / distance) * overlap * 0.5f;
                float pushY = (dy / distance) * overlap * 0.5f;

                enemy1->SetPosition(rect1.x + pushX, rect1.y + pushY);
                enemy2->SetPosition(rect2.x - pushX, rect2.y - pushY);

                enemy1->SetVelocity(enemy1->GetVelocityX() * 0.2f, enemy1->GetVelocityY() * 0.2f);
                enemy2->SetVelocity(enemy2->GetVelocityX() * 0.2f, enemy2->GetVelocityY() * 0.2f);
            }
        }
    }
}

void EnemyManager::UpdateRangeRover(float deltaTime)
{
    for (auto& rangeRover : rangeRovers)
    {
        rangeRover->Update(deltaTime);
    }

    for (size_t i = 0; i < rangeRovers.size(); ++i)
    {
        for (size_t j = i + 1; j < rangeRovers.size(); ++j)
        {
            auto& enemy1 = rangeRovers[i];
            auto& enemy2 = rangeRovers[j];

            SDL_FRect rect1 = enemy1->GetCollisionRect();
            SDL_FRect rect2 = enemy2->GetCollisionRect();

            if (CheckCollision(rect1, rect2))
            {
                float dx = (rect1.x + rect1.w / 2) - (rect2.x + rect2.w / 2);
                float dy = (rect1.y + rect1.h / 2) - (rect2.y + rect2.h / 2);
                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance == 0) distance = 0.001f;

                float overlap = (rect1.w / 2 + rect2.w / 2) - distance;
                float pushX = (dx / distance) * overlap * 0.5f;
                float pushY = (dy / distance) * overlap * 0.5f;

                enemy1->SetPosition(rect1.x + pushX, rect1.y + pushY);
                enemy2->SetPosition(rect2.x - pushX, rect2.y - pushY);

                enemy1->SetVelocity(enemy1->GetVelocityX() * 0.2f, enemy1->GetVelocityY() * 0.2f);
                enemy2->SetVelocity(enemy2->GetVelocityX() * 0.2f, enemy2->GetVelocityY() * 0.2f);
            }
        }
    }

    for (auto& rangeRover : rangeRovers)
    {
        SDL_FRect rangeRoverRect = rangeRover->GetCollisionRect();

        for (auto& enemy : enemies)
        {
            SDL_FRect enemyRect = enemy->GetCollisionRect();

            if (CheckCollision(rangeRoverRect, enemyRect))
            {
                float dx = (rangeRoverRect.x + rangeRoverRect.w / 2) - (enemyRect.x + enemyRect.w / 2);
                float dy = (rangeRoverRect.y + rangeRoverRect.h / 2) - (enemyRect.y + enemyRect.h / 2);
                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance == 0) distance = 0.001f;

                float overlap = (rangeRoverRect.w / 2 + enemyRect.w / 2) - distance;
                float pushX = (dx / distance) * overlap * 0.5f;
                float pushY = (dy / distance) * overlap * 0.5f;

                // Przesuñ tylko Enemy, RangeRover pozostaje nieruszony
                enemy->SetPosition(enemyRect.x - pushX, enemyRect.y - pushY);

                enemy->SetVelocity(enemy->GetVelocityX() * 0.2f, enemy->GetVelocityY() * 0.2f);
            }
        }
    }
}

void EnemyManager::Render(SDL_Renderer* renderer)
{
    for (auto& enemy : enemies)
    {
        enemy->Render(renderer);
    }
}

void EnemyManager::RenderRangeRover(SDL_Renderer* renderer)
{
    for (auto& rangeRover : rangeRovers)
    {
        rangeRover->Render(renderer);
    }
}

float EnemyManager::GetRandomFloat(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

bool EnemyManager::CheckCollision(const SDL_FRect& rect1, const SDL_FRect& rect2) const
{
    float center1X = rect1.x + rect1.w / 2;
    float center1Y = rect1.y + rect1.h / 2;

    float center2X = rect2.x + rect2.w / 2;
    float center2Y = rect2.y + rect2.h / 2;

    float radius1 = rect1.w / 2;
    float radius2 = rect2.w / 2;

    float dx = center1X - center2X;
    float dy = center1Y - center2Y;
    float distance = std::sqrt(dx * dx + dy * dy);

    return distance < (radius1 + radius2);
}