#pragma once

// RangeRover odpowiada za krêc¹cego siê ziuta

#include "Enemy.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "Camera.hpp"
#include "Settings.hpp"
#include <SDL3/SDL.h>
#include <vector>

class RangeRover
{
public:
	RangeRover(Player* player, Map* map, Camera* camera, SDL_Renderer* renderer); // Konstruktor RangeRovera, gracz, mapa, kamera, renderer
	~RangeRover(); // Destruktor

	void LoadTexture(SDL_Renderer* renderer, const char* pathFile); // Za³adowanie tekstury RangeRovera
	void Render(SDL_Renderer* renderer); // Renderowanie RangeRovera
	void SetPosition(float x, float y); // Ustwienie pozycji RangeRovera
	void Update(float deltaTime); // Aktualizacja RangeRovera
	void SetVelocity(float x, float y); // Ustwienie prêdkoœci RangeRovera
	float GetX() const; // Pobranie pozycji x
	float GetY() const; // Pobranie pozycji y
	float GetVelocityX() const;	// Pobranie prêdkoœci x
	float GetVelocityY() const; // Pobranie prêdkoœci y
	void SetCurrentRow(int row); // Ustwienie aktualnego wiersza dla animacji

	SDL_FRect GetCollisionRect() const; // Pobranie prostok¹ta kolizji

	static float numOfRangeRovers; // Liczba RangeRoverów

private:
	Player* player; // Gracz
	Map* map; // Mapa
	Camera* camera; // Kamera
	Enemy* enemy; // Przeciwnik

	void UpdateAnimation(); // Aktualizacja animacji

	float x; // Pozycja x
	float y; // Pozycja y
	float velocityX; // Prêdkoœæ x
	float velocityY; // Prêdkoœæ y
	float directionX; // Kierunek x
	float directionY; // Kierunek y
	float speed; // Prêdkoœæ
	SDL_Texture* playerTexture; // Tekstura RangeRovera
	int frameWidth; // Szerokoœæ klatki
	int frameHeight; // Wysokoœæ klatki
	int currentFrame; // Aktualna klatka
	int currentRow; // Aktualny wiersz
	int totalFrames; // Ca³kowita liczba klatek
	Uint64 lastFrameTime; // Ostatni czas klatki
	Uint64 frameDuration; // Czas trwania klatki

	float angle; // K¹t
	float radius; // Promieñ
};