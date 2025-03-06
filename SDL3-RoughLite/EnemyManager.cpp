#define _USE_MATH_DEFINES

#include "EnemyManager.hpp"
#include <iostream>

long int i = 1;
extern GameStateRunning gameState;

EnemyManager::EnemyManager(Player* player, Map* map, Camera* camera, SDL_Renderer* renderer)
	: player(player), map(map), camera(camera), renderer(renderer), lastSpawnTime(SDL_GetTicks()), lastCollisionTime(SDL_GetTicks())
{
}

void EnemyManager::AdjustSpawnTime(Uint64 pauseDuration)
{
	lastSpawnTime += pauseDuration;
	lastCollisionTime += pauseDuration;
}

void EnemyManager::AddEnemy() // Dodanie przeciwnika
{
	float enemyX, enemyY; // Pozycja x, y przeciwnika
	bool validPosition = false; // Czy pozycja jest poprawna

	while (!validPosition) // Dop�ki pozycja nie jest poprawna
    {
		enemyX = GetRandomFloat(0, mapWidth - enemyW); // Ustawienie losowej pozycji x przeciwnika
		enemyY = GetRandomFloat(0, mapHeight - enemyH); // Ustawienie losowej pozycji y przeciwnika

		validPosition = true; // Ustawienie pozycji na poprawn�
		SDL_FRect newEnemyRect = { enemyX, enemyY, enemyW, enemyH }; // Utworzenie nowego prostok�ta kolizji

		for (const auto& enemy : enemies) // Dla ka�dego przeciwnika
		{
			if (CheckCollision(newEnemyRect, enemy->GetCollisionRect())) // Je�li kolizja
			{
				validPosition = false; // Ustawienie pozycji na niepoprawn�
				break; // Przerwanie
			}
		}
    }

	auto enemy = std::make_unique<Enemy>(player, map, camera, renderer); // Utworzenie nowego przeciwnika
	enemy->SetPosition(enemyX, enemyY); // Ustawienie pozycji przeciwnika
	enemies.push_back(std::move(enemy)); // Dodanie przeciwnika
}

void EnemyManager::AddBigEnemy() // Dodanie przeciwnika
{
	float enemyX, enemyY; // Pozycja x, y przeciwnika
	bool validPosition = false; // Czy pozycja jest poprawna

	while (!validPosition) // Dop�ki pozycja nie jest poprawna
	{
		enemyX = GetRandomFloat(0, mapWidth - enemyW); // Ustawienie losowej pozycji x przeciwnika
		enemyY = GetRandomFloat(0, mapHeight - enemyH); // Ustawienie losowej pozycji y przeciwnika

		validPosition = true; // Ustawienie pozycji na poprawn�
		SDL_FRect newEnemyRect = { enemyX, enemyY, enemyW * 2, enemyH * 2}; // Utworzenie nowego prostok�ta kolizji

		for (const auto& enemy : enemies) // Dla ka�dego przeciwnika
		{
			if (CheckCollision(newEnemyRect, enemy->GetCollisionRect())) // Je�li kolizja
			{
				validPosition = false; // Ustawienie pozycji na niepoprawn�
				break; // Przerwanie
			}
		}
	}

	auto enemy = std::make_unique<Enemy>(player, map, camera, renderer); // Utworzenie nowego przeciwnika
	enemy->SetPosition(enemyX, enemyY); // Ustawienie pozycji przeciwnika
	bigEnemies.push_back(std::move(enemy)); // Dodanie przeciwnika
}

void EnemyManager::Update(float deltaTime, GameStateRunning currentState) // Aktualizacja przeciwnika
{
	if (currentState != GameStateRunning::GAME || player->isGameOver)
	{
		SDL_Log("EnemyManager::Update - Nie aktualizuj� przeciwnik�w, poniewa� currentState = %d", static_cast<int>(currentState));
		return;
	}

	else
	{
		Uint64 currentTime = SDL_GetTicks();

		if (currentTime > lastSpawnTime + 1000)
		{
			AddEnemy();
			lastSpawnTime = currentTime;
		}

		if (currentTime > lastBigSpawnTime + 100000)
		{
			AddBigEnemy();
			lastBigSpawnTime = currentTime;
		}

		for (auto& enemy : enemies)
		{
			enemy->Update(deltaTime, currentState);
		}

		for (auto& enemy : bigEnemies)
		{
			enemy->Update(deltaTime, currentState);
		}

		enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const std::unique_ptr<Enemy>& enemy) { return !enemy->isAlive; }), enemies.end());

		for (size_t i = 0; i < enemies.size(); ++i) // Dla ka�dego przeciwnika
		{
			for (size_t j = i + 1; j < enemies.size(); ++j) // Dla ka�dego przeciwnika
			{
				auto& enemy1 = enemies[i]; // Przeciwnik 1
				auto& enemy2 = enemies[j]; // Przeciwnik 2

				SDL_FRect rect1 = enemy1->GetCollisionRect(); // Prostok�t kolizji 1
				SDL_FRect rect2 = enemy2->GetCollisionRect(); // Prostok�t kolizji 2

				if (CheckCollision(rect1, rect2)) // Je�li kolizja
				{
					float dx = (rect1.x + rect1.w / 2) - (rect2.x + rect2.w / 2); // R�nica x
					float dy = (rect1.y + rect1.h / 2) - (rect2.y + rect2.h / 2); // R�nica y

					float distance = std::sqrt(dx * dx + dy * dy); // Odleg�o��
					if (distance == 0) distance = 0.001f; // Je�li odleg�o�� jest r�wna 0

					float overlap = (rect1.w / 2 + rect2.w / 2) - distance; // Przeci�cie
					float pushX = (dx / distance) * overlap * 0.5f; // Przesuni�cie x
					float pushY = (dy / distance) * overlap * 0.5f; // Przesuni�cie y

					enemy1->SetPosition(rect1.x + pushX, rect1.y + pushY); // Ustawienie pozycji
					enemy2->SetPosition(rect2.x - pushX, rect2.y - pushY); // Ustawienie pozycji

					enemy1->SetVelocity(enemy1->GetVelocityX() * 0.2f, enemy1->GetVelocityY() * 0.2f); // Ustawienie pr�dko�ci
					enemy2->SetVelocity(enemy2->GetVelocityX() * 0.2f, enemy2->GetVelocityY() * 0.2f); // Ustawienie pr�dko�ci
				}
			}
		}

		if (IsPlayerInCollision() && (currentTime > lastCollisionTime + 1000))
		{
			player->health -= 10;
			lastCollisionTime = currentTime;
			player->renderHealthBar(player->health, renderer);
		}
	}
}

void EnemyManager::Render(SDL_Renderer* renderer, GameStateRunning currentState) // Renderowanie przeciwnika
{
	if (currentState != GameStateRunning::GAME)
	{
		return;
	}

	for (auto& enemy : enemies) // Dla ka�dego przeciwnika
	{
		enemy->Render(renderer); // Renderowanie
	}

	for (auto& enemy : bigEnemies) // Dla ka�dego przeciwnika
	{
		enemy->BigRender(renderer); // Renderowanie
	}
}

float EnemyManager::GetRandomFloat(float min, float max) // Pobranie losowej liczby zmiennoprzecinkowej
{ 
	static std::random_device rd; // Urz�dzenie losowe
	static std::mt19937 gen(rd()); // Generator
	std::uniform_real_distribution<float> dis(min, max); // Rozk�ad jednostajny
	return dis(gen); // Zwr�� losow� liczb�
}

bool EnemyManager::CheckCollision(const SDL_FRect& rect1, const SDL_FRect& rect2) const // Sprawdzenie kolizji
{
	float center1X = rect1.x + rect1.w / 2; // �rodek x
	float center1Y = rect1.y + rect1.h / 2; // �rodek y

	float center2X = rect2.x + rect2.w / 2; // �rodek x
	float center2Y = rect2.y + rect2.h / 2; // �rodek y

	float radius1 = rect1.w / 2; // Promie�
    float radius2 = rect2.w / 2; // Peomie�	

	float dx = center1X - center2X; // R�nica x
	float dy = center1Y - center2Y; // R�nica y
	float distance = std::sqrt(dx * dx + dy * dy); // Odleg�o��

	return distance < (radius1 + radius2);	// Zwr�� czy odleg�o�� jest mniejsza od sumy promieni
}

bool EnemyManager::IsPlayerInCollision() const // Czy gracz jest w kolizji
{
	SDL_FRect playerRect = player->GetCollisionRect(); // Pobierz prostok�t kolizji gracza

	for (const auto& enemy : enemies) // Dla ka�dego przeciwnika
	{
		SDL_FRect enemyRect = enemy->GetCollisionRect(); // Pobierz prostok�t kolizji przeciwnika
		if (CheckCollision(playerRect, enemyRect)) // Sprawd� kolizj�
		{
			return true; // Je�li kolizja, zwr�� true
		}
	}

	return false; // Je�li brak kolizji, zwr�� false
}

void EnemyManager::Reset()
{
	// Usu� wszystkich przeciwnik�w
	enemies.clear();
	// Dodaj przeciwnik�w ponownie, je�li to konieczne
	AddEnemy();
}