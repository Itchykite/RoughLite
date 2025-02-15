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