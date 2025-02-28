#define _USE_MATH_DEFINES

#include "EnemyManager.hpp"
#include "GameState.hpp"
#include <iostream>

long int i = 1;
extern GameStateRunning gameState;

EnemyManager::EnemyManager(Player* player, Map* map, Camera* camera, SDL_Renderer* renderer) // Konstruktor przeciwnika, gracz, mapa, kamera, renderer
    : player(player), map(map), camera(camera), renderer(renderer)
{
}

void EnemyManager::AddEnemy() // Dodanie przeciwnika
{
	float enemyX, enemyY; // Pozycja x, y przeciwnika
	bool validPosition = false; // Czy pozycja jest poprawna

	while (!validPosition) // Dopóki pozycja nie jest poprawna
    {
		enemyX = GetRandomFloat(0, mapWidth - enemyW); // Ustawienie losowej pozycji x przeciwnika
		enemyY = GetRandomFloat(0, mapHeight - enemyH); // Ustawienie losowej pozycji y przeciwnika

		validPosition = true; // Ustawienie pozycji na poprawn¹
		SDL_FRect newEnemyRect = { enemyX, enemyY, enemyW, enemyH }; // Utworzenie nowego prostok¹ta kolizji

		for (const auto& enemy : enemies) // Dla ka¿dego przeciwnika
		{
			if (CheckCollision(newEnemyRect, enemy->GetCollisionRect())) // Jeœli kolizja
			{
				validPosition = false; // Ustawienie pozycji na niepoprawn¹
				break; // Przerwanie
			}
		}
    }

	auto enemy = std::make_unique<Enemy>(player, map, camera, renderer); // Utworzenie nowego przeciwnika
	enemy->SetPosition(enemyX, enemyY); // Ustawienie pozycji przeciwnika
	enemies.push_back(std::move(enemy)); // Dodanie przeciwnika
}

void EnemyManager::AddRangeRover() // Dodanie RangeRovera
{
	float angleStep = 360.0f / RangeRover::numOfRangeRovers; // Krok k¹towy
	float radius = 200.0f; // Promieñ

	for (int i = 0; i < RangeRover::numOfRangeRovers; ++i) // Dla ka¿dego RangeRovera
    {
		float angle = i * angleStep; // K¹t
		float enemyX = player->GetX() + radius * std::cos(angle * M_PI / 180.0f); // Pozycja x
		float enemyY = player->GetY() + radius * std::sin(angle * M_PI / 180.0f); // Pozycja y

		auto rangeRover = std::make_unique<RangeRover>(player, map, camera, renderer); // Utworzenie nowego RangeRovera
		rangeRover->SetPosition(enemyX, enemyY); // Ustawienie pozycji
		rangeRovers.push_back(std::move(rangeRover)); // Dodanie RangeRovera
    }
}

void EnemyManager::Update(float deltaTime) // Aktualizacja przeciwnika
{
	if (gameState == GameStateRunning::MENU)
	{
		return;
	}

	Uint64 currentTime = SDL_GetTicks(); // Aktualny czas

	if (currentTime > lastSpawnTime + 1000) // Jeœli aktualny czas jest wiêkszy od ostatniego czasu + 1000
	{
		AddEnemy(); // Dodanie przeciwnika
		lastSpawnTime = currentTime; // Ustawienie ostatniego czasu
	}

	for (auto& enemy : enemies) // Dla ka¿dego przeciwnika
	{
		enemy->Update(deltaTime); // Aktualizacja
	}

	enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const std::unique_ptr<Enemy>& enemy) { return !enemy->isAlive; }), enemies.end());

	for (size_t i = 0; i < enemies.size(); ++i) // Dla ka¿dego przeciwnika
	{
		for (size_t j = i + 1; j < enemies.size(); ++j) // Dla ka¿dego przeciwnika
		{
			auto& enemy1 = enemies[i]; // Przeciwnik 1
			auto& enemy2 = enemies[j]; // Przeciwnik 2

			SDL_FRect rect1 = enemy1->GetCollisionRect(); // Prostok¹t kolizji 1
			SDL_FRect rect2 = enemy2->GetCollisionRect(); // Prostok¹t kolizji 2

            if (CheckCollision(rect1, rect2)) // Jeœli kolizja
            {
                float dx = (rect1.x + rect1.w / 2) - (rect2.x + rect2.w / 2); // Ró¿nica x
                float dy = (rect1.y + rect1.h / 2) - (rect2.y + rect2.h / 2); // Ró¿nica y

                float distance = std::sqrt(dx * dx + dy * dy); // Odleg³oœæ
                if (distance == 0) distance = 0.001f; // Jeœli odleg³oœæ jest równa 0

                float overlap = (rect1.w / 2 + rect2.w / 2) - distance; // Przeciêcie
                float pushX = (dx / distance) * overlap * 0.5f; // Przesuniêcie x
                float pushY = (dy / distance) * overlap * 0.5f; // Przesuniêcie y

                enemy1->SetPosition(rect1.x + pushX, rect1.y + pushY); // Ustawienie pozycji
                enemy2->SetPosition(rect2.x - pushX, rect2.y - pushY); // Ustawienie pozycji

                enemy1->SetVelocity(enemy1->GetVelocityX() * 0.2f, enemy1->GetVelocityY() * 0.2f); // Ustawienie prêdkoœci
                enemy2->SetVelocity(enemy2->GetVelocityX() * 0.2f, enemy2->GetVelocityY() * 0.2f); // Ustawienie prêdkoœci
            }
        }
    }

	if (IsPlayerInCollision() && (currentTime > lastCollisionTime + 1000)) // Jeœli kolizja i minê³o 1000 ms od ostatniej kolizji
	{
		player->health -= 10; // Zmniejszenie zdrowia gracza
		lastCollisionTime = currentTime; // Ustawienie ostatniego czasu kolizji
		player->renderHealthBar(player->health, renderer); // Renderowanie paska zdrowia
	}
}

void EnemyManager::UpdateRangeRover(float deltaTime) // Aktualizacja RangeRovera
{
	for (auto& rangeRover : rangeRovers) // Dla ka¿dego RangeRovera
    { 
		rangeRover->Update(deltaTime); // Aktualizacja
    }

	for (size_t i = 0; i < rangeRovers.size(); ++i) // Dla ka¿dego RangeRovera
	{
		for (size_t j = i + 1; j < rangeRovers.size(); ++j) // Dla ka¿dego RangeRovera
		{
			auto& enemy1 = rangeRovers[i]; // RangeRover 1
			auto& enemy2 = rangeRovers[j]; // RangeRover 2

			SDL_FRect rect1 = enemy1->GetCollisionRect(); // Prostok¹t kolizji 1
			SDL_FRect rect2 = enemy2->GetCollisionRect(); // Prostok¹t kolizji 2

            if (CheckCollision(rect1, rect2)) // Jeœli kolizja
            {
                float dx = (rect1.x + rect1.w / 2) - (rect2.x + rect2.w / 2); // Ró¿nica x
                float dy = (rect1.y + rect1.h / 2) - (rect2.y + rect2.h / 2); // Ró¿nica y

                float distance = std::sqrt(dx * dx + dy * dy); // Odleg³oœæ
                if (distance == 0) distance = 0.001f; // Jeœli odleg³oœæ jest równa 0

                float overlap = (rect1.w / 2 + rect2.w / 2) - distance; // Przeciêcie
                float pushX = (dx / distance) * overlap * 0.5f; // Przesuniêcie x
                float pushY = (dy / distance) * overlap * 0.5f; // Przesuniêcie y

                enemy1->SetPosition(rect1.x + pushX, rect1.y + pushY); // Ustawienie pozycji
                enemy2->SetPosition(rect2.x - pushX, rect2.y - pushY); // Ustawienie pozycji

                enemy1->SetVelocity(enemy1->GetVelocityX() * 0.2f, enemy1->GetVelocityY() * 0.2f); // Ustawienie prêdkoœci
                enemy2->SetVelocity(enemy2->GetVelocityX() * 0.2f, enemy2->GetVelocityY() * 0.2f); // Ustawienie prêdkoœci
            }
        }
    }

	for (auto& rangeRover : rangeRovers) // Dla ka¿dego RangeRovera
    {
		SDL_FRect rangeRoverRect = rangeRover->GetCollisionRect(); // Prostok¹t kolizji RangeRovera

		for (auto& enemy : enemies) // Dla ka¿dego przeciwnika
        {
			SDL_FRect enemyRect = enemy->GetCollisionRect(); // Prostok¹t kolizji przeciwnika

			if (CheckCollision(rangeRoverRect, enemyRect)) // Jeœli kolizja
            {
				float dx = (rangeRoverRect.x + rangeRoverRect.w / 2) - (enemyRect.x + enemyRect.w / 2); // Ró¿nica x
				float dy = (rangeRoverRect.y + rangeRoverRect.h / 2) - (enemyRect.y + enemyRect.h / 2); // Ró¿nica y
				float distance = std::sqrt(dx * dx + dy * dy); // Odleg³oœæ

				if (distance == 0) distance = 0.001f; // Jeœli odleg³oœæ jest równa 0

				float overlap = (rangeRoverRect.w / 2 + enemyRect.w / 2) - distance; // Przeciêcie
				float pushX = (dx / distance) * overlap * 0.5f; // Przesuniêcie x
				float pushY = (dy / distance) * overlap * 0.5f; // Przesuniêcie y

                // Przesuñ tylko Enemy, RangeRover pozostaje nieruszony
                enemy->SetPosition(enemyRect.x - pushX, enemyRect.y - pushY);

				enemy->SetVelocity(enemy->GetVelocityX() * 0.2f, enemy->GetVelocityY() * 0.2f); // Ustawienie prêdkoœci
            }
        }
    }
}

void EnemyManager::Render(SDL_Renderer* renderer) // Renderowanie przeciwnika
{
	for (auto& enemy : enemies) // Dla ka¿dego przeciwnika
    {
		enemy->Render(renderer); // Renderowanie
    }
}

void EnemyManager::RenderRangeRover(SDL_Renderer* renderer) // Renderowanie RangeRovera
{
	for (auto& rangeRover : rangeRovers) // Dla ka¿dego RangeRovera
    {
		rangeRover->Render(renderer); // Renderowanie
    }
}

float EnemyManager::GetRandomFloat(float min, float max) // Pobranie losowej liczby zmiennoprzecinkowej
{ 
	static std::random_device rd; // Urz¹dzenie losowe
	static std::mt19937 gen(rd()); // Generator
	std::uniform_real_distribution<float> dis(min, max); // Rozk³ad jednostajny
	return dis(gen); // Zwróæ losow¹ liczbê
}

bool EnemyManager::CheckCollision(const SDL_FRect& rect1, const SDL_FRect& rect2) const // Sprawdzenie kolizji
{
	float center1X = rect1.x + rect1.w / 2; // Œrodek x
	float center1Y = rect1.y + rect1.h / 2; // Œrodek y

	float center2X = rect2.x + rect2.w / 2; // Œrodek x
	float center2Y = rect2.y + rect2.h / 2; // Œrodek y

	float radius1 = rect1.w / 2; // Promieñ
    float radius2 = rect2.w / 2; // Peomieñ	

	float dx = center1X - center2X; // Ró¿nica x
	float dy = center1Y - center2Y; // Ró¿nica y
	float distance = std::sqrt(dx * dx + dy * dy); // Odleg³oœæ

	return distance < (radius1 + radius2);	// Zwróæ czy odleg³oœæ jest mniejsza od sumy promieni
}

bool EnemyManager::IsPlayerInCollision() const // Czy gracz jest w kolizji
{
	SDL_FRect playerRect = player->GetCollisionRect(); // Pobierz prostok¹t kolizji gracza

	for (const auto& enemy : enemies) // Dla ka¿dego przeciwnika
	{
		SDL_FRect enemyRect = enemy->GetCollisionRect(); // Pobierz prostok¹t kolizji przeciwnika
		if (CheckCollision(playerRect, enemyRect)) // SprawdŸ kolizjê
		{
			return true; // Jeœli kolizja, zwróæ true
		}
	}

	return false; // Jeœli brak kolizji, zwróæ false
}
