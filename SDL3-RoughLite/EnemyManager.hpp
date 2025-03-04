#pragma once

// EnemyManager odpowiada za zarz¹dzanie przeciwnikami

#include <SDL3/SDL.h>
#include <memory>
#include <vector>
#include <random>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Settings.hpp"
#include "RangeRover.hpp"
#include "GameState.hpp"

class EnemyManager
{
public:
	EnemyManager(Player* player, Map* map, Camera* camera, SDL_Renderer* renderer); // Konstrutkor przeciwnika, gracz, mapa, kamera, renderer

	void AddEnemy(); // Dodanie przeciwnika
	void AddRangeRover(); // Dodanie RangeRovera

	void Update(float deltaTime, GameStateRunning currentState); // Aktualizacja przeciwnika
	void Render(SDL_Renderer* renderer, GameStateRunning currentState); // Renderowanie przeciwnika

	void AdjustSpawnTime(Uint64 pauseDuration); // Nowa metoda

	std::vector<std::unique_ptr<Enemy>> enemies; // Przeciwnicy

	Player* player; // Gracz
	Map* map; // Mapa
	Camera* camera; // Kamera
	SDL_Renderer* renderer; // Renderer

private:
	bool CheckCollision(const SDL_FRect& rect1, const SDL_FRect& rect2) const; // Sprawdzenie kolizji
	bool IsPlayerInCollision() const; // Czy gracz jest w kolizji

	float GetRandomFloat(float min, float max); // Pobranie losowej liczby zmiennoprzecinkowej

	Uint64 lastSpawnTime = 0; // Ostatni czas spawnu przeciwnika
	Uint64 lastCollisionTime = 0; // Ostatni czas spawnu przeciwnika
};