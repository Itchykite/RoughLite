#pragma once

// Enemy odpowiada za przeciwników, ich zachowanie, wygl¹d, kolizje

#include "Player.hpp"
#include "Map.hpp"
#include "Camera.hpp"
#include "Map.hpp"

#include <SDL3/SDL.h>
#include <vector>

class Enemy
{
public:
	Enemy(Player* player, Map* map, Camera* camera, SDL_Renderer* renderer); // Konstruktor przeciwnika, gracz, mapa, kamera, renderer
	~Enemy(); // Destruktor

	void LoadTexture(SDL_Renderer* renderer, const char* pathFile); // Za³adowanie tekstury przeciwnika
	void Render(SDL_Renderer* renderer); // Renderowanie przeciwnika
	void SetPosition(float x, float y); // Ustwienie pozycji przeciwnika
	void Update(float deltaTime); // Aktualizacja przeciwnika
	void SetVelocity(float x, float y); // Ustwienie prêdkoœci przeciwnika
	float GetX() const; // Pobranie pozycji x
	float GetY() const; // Pobranie pozycji y
	float GetVelocityX() const; // Pobranie prêdkoœci x
	float GetVelocityY() const; // Pobranie prêdkoœci y
	void SetCurrentRow(int row); // Ustwienie aktualnego wiersza dla animacji

	SDL_FRect GetCollisionRect() const; // Pobranie prostok¹ta kolizji

	static float playerW; // Szerokoœæ gracza
	static float playerH; // Wysokoœæ gracza

	static float enemyW; // Szerokoœæ przeciwnika
	static float enemyH; // Wysokoœæ przeciwnika

private:
	Player* player; // Gracz
	Map* map; // Mapa
	Camera* camera; // Kamera

	void UpdateAnimation(); // Aktualizacja animacji

	float x; // Pozycja x
	float y; // Pozycja y
	float velocityX; // Prêdkoœæ x
	float velocityY; // Prêdkoœæ y
	float directionX; // Kierunek x
	float directionY; // Kierunek y
	float speed; // Prêdkoœæ
	SDL_Texture* playerTexture; // Tekstura przeciwnika
	int frameWidth; // Szerokoœæ klatki
	int frameHeight; // Wysokoœæ klatki
	int currentFrame; // Aktualna klatka
	int currentRow; // Aktualny wiersz
	int totalFrames; // Ca³kowita liczba klatek
	Uint32 lastFrameTime; // Ostatni czas klatki
	Uint32 frameDuration; // Czas trwania klatki

	double health; // Zdrowie
};