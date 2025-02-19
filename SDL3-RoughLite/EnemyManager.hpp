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

class EnemyManager
{
public:
	EnemyManager(Player* player, Map* map, Camera* camera, SDL_Renderer* renderer); // Konstrutkor przeciwnika, gracz, mapa, kamera, renderer

	void AddEnemy(); // Dodanie przeciwnika
	void AddRangeRover(); // Dodanie RangeRovera

	void Update(float deltaTime); // Aktualizacja przeciwnika
	void UpdateRangeRover(float deltaTime); // Aktualizacja RangeRovera

	void Render(SDL_Renderer* renderer); // Renderowanie przeciwnika
	void RenderRangeRover(SDL_Renderer* renderer); // Renderowanie RangeRovera

private:
	bool CheckCollision(const SDL_FRect& rect1, const SDL_FRect& rect2) const; // Sprawdzenie kolizji
	std::vector<std::unique_ptr<Enemy>> enemies; // Przeciwnicy
	std::vector<std::unique_ptr<RangeRover>> rangeRovers; // RangeRover
	Player* player; // Gracz
	Map* map; // Mapa
	Camera* camera; // Kamera

	SDL_Renderer* renderer; // Renderer

	float GetRandomFloat(float min, float max); // Pobranie losowej liczby zmiennoprzecinkowej
};