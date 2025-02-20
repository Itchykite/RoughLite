#include "RangeRover.hpp"
#include "Map.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "Settings.hpp"
#include <SDL3_image/SDL_image.h>
#include <cmath>

float RangeRover::numOfRangeRovers = 1; // Liczba RangeRoverów

RangeRover::RangeRover(Player* player, Map* map, Camera* camera, SDL_Renderer* renderer) // Konstruktor RangeRovera, gracz, mapa, kamera, renderer
	: x(0), y(0), velocityX(0), velocityY(0), speed(10.0f), playerTexture(nullptr), 
	frameWidth(0), frameHeight(0), currentFrame(0), currentRow(0),
	totalFrames(9), lastFrameTime(0), frameDuration(100), player(player), map(map), camera(camera), 
	angle(0.0f), radius(200.0f)
{
	LoadTexture(renderer, "spritesheet.png");
}

RangeRover::~RangeRover() // Destruktor
{
	if (playerTexture) // Jeœli tekstura gracza istnieje
	{
		SDL_DestroyTexture(playerTexture); // Usuniêcie tekstury gracza
		playerTexture = nullptr;
	}
}

void RangeRover::LoadTexture(SDL_Renderer* renderer, const char* pathFile) // Za³adowanie tekstury RangeRovera
{
	SDL_Surface* surface = IMG_Load(pathFile); // Wczytanie tekstury RangeRovera
	if (!surface) // Jeœli tekstura RangeRovera nie istnieje
	{
		SDL_Log("Couldn't load range rover texture: %s", SDL_GetError());
		return;
	}

	playerTexture = SDL_CreateTextureFromSurface(renderer, surface); // Utworzenie tekstury RangeRovera
	if (!playerTexture) // Jeœli tekstura RangeRovera nie istnieje
	{
		SDL_Log("Couldn't create texture from surface: %s", SDL_GetError());
	}

	frameWidth = surface->w / 9; // Szerokoœæ klatki
	frameHeight = surface->h / 4; // Wysokoœæ klatki
	totalFrames = 4; // Ca³kowita liczba klatek

	SDL_DestroySurface(surface); // Usuniêcie powierzchni
}

void RangeRover::Render(SDL_Renderer* renderer) // Renderowanie RangeRovera
{
	if (camera == nullptr) // Jeœli kamera nie istnieje
	{
		SDL_Log("Enemy::Render() - camera is nullptr");
		return;
	}

	SDL_FRect srcRect = { currentFrame * frameWi
		dth, currentRow * frameHeight, frameWidth, frameHeight }; // Ustawienie klatki
	SDL_FRect dstRect = { x - camera->GetX() + (playerW / 2), y - camera->GetY() + (playerH / 2), enemyW, enemyH }; // Ustawienie pozycji
	SDL_RenderTexture(renderer, playerTexture, &srcRect, &dstRect); // Renderowanie tekstury
}

void RangeRover::SetPosition(float x, float y) 
{
	this->x = x; // Ustaw pozycjê x
	this->y = y; // Ustaw pozycjê y
}

void RangeRover::Update(float deltaTime) // Aktualizacja RangeRovera
{
	if (player == nullptr) // Jeœli gracz nie istnieje
	{
		SDL_Log("Enemy::Update() - player is nullptr");
		return;
	}

	if (map == nullptr) // Jeœli mapa nie istnieje
	{
		SDL_Log("Enemy::Update() - map is nullptr");
		return;
	}

	float targetX = player->GetX(); // Pozycja x gracza
	float targetY = player->GetY(); // Pozycja y gracza

	angle += speed * deltaTime; // K¹t
	if (angle > 360.0f) // Jeœli k¹t jest wiêkszy od 360
	{
		angle -= 360.0f; // Odejmij 360
	}

	x = targetX + radius * std::cos(angle); // Pozycja x
	y = targetY + radius * std::sin(angle); // Pozycja y 

	UpdateAnimation(); // Aktualizacja animacji
}

void RangeRover::SetVelocity(float x, float y)
{
	velocityX = x; // Ustaw prêdkoœæ x
	velocityY = y; // Ustaw prêdkoœæ y
}

float RangeRover::GetX() const
{
	return x;
}

float RangeRover::GetY() const
{
	return y;
}

float RangeRover::GetVelocityX() const
{
	return velocityX;
}

float RangeRover::GetVelocityY() const
{
	return velocityY;
}

void RangeRover::SetCurrentRow(int row)
{
	currentRow = row;
}

void RangeRover::UpdateAnimation() // Aktualizacja animacji
{
	if (velocityX == 0 && velocityY == 0) // Jeœli prêdkoœæ x i y s¹ równe 0
	{
		return;
	}

	Uint32 currentTime = SDL_GetTicks(); // Aktualny czas
	if (currentTime > lastFrameTime + frameDuration) // Jeœli aktualny czas jest wiêkszy od ostatniego czasu klatki + czasu trwania klatki
	{
		currentFrame = (currentFrame + 1) % totalFrames; // Ustaw klatkê
		lastFrameTime = currentTime; // Ustaw ostatni czas klatki
	}
}

SDL_FRect RangeRover::GetCollisionRect() const 
{
	return SDL_FRect
	{
		x,
		y,
		enemyW / 2,
		enemyH / 2
	}; // Zwróæ prostok¹t kolizji
}