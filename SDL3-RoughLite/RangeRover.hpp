#pragma once

// RangeRover odpowiada za kr�c�cego si� ziuta

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

	void LoadTexture(SDL_Renderer* renderer, const char* pathFile); // Za�adowanie tekstury RangeRovera
	void Render(SDL_Renderer* renderer); // Renderowanie RangeRovera
	void SetPosition(float x, float y); // Ustwienie pozycji RangeRovera
	void Update(float deltaTime); // Aktualizacja RangeRovera
	void SetVelocity(float x, float y); // Ustwienie pr�dko�ci RangeRovera
	float GetX() const; // Pobranie pozycji x
	float GetY() const; // Pobranie pozycji y
	float GetVelocityX() const;	// Pobranie pr�dko�ci x
	float GetVelocityY() const; // Pobranie pr�dko�ci y
	void SetCurrentRow(int row); // Ustwienie aktualnego wiersza dla animacji

	SDL_FRect GetCollisionRect() const; // Pobranie prostok�ta kolizji

	static float numOfRangeRovers; // Liczba RangeRover�w

private:
	Player* player; // Gracz
	Map* map; // Mapa
	Camera* camera; // Kamera
	Enemy* enemy; // Przeciwnik

	void UpdateAnimation(); // Aktualizacja animacji

	float x; // Pozycja x
	float y; // Pozycja y
	float velocityX; // Pr�dko�� x
	float velocityY; // Pr�dko�� y
	float directionX; // Kierunek x
	float directionY; // Kierunek y
	float speed; // Pr�dko��
	SDL_Texture* playerTexture; // Tekstura RangeRovera
	int frameWidth; // Szeroko�� klatki
	int frameHeight; // Wysoko�� klatki
	int currentFrame; // Aktualna klatka
	int currentRow; // Aktualny wiersz
	int totalFrames; // Ca�kowita liczba klatek
	Uint64 lastFrameTime; // Ostatni czas klatki
	Uint64 frameDuration; // Czas trwania klatki

	float angle; // K�t
	float radius; // Promie�
};