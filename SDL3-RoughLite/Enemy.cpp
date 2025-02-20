#include "Enemy.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "Settings.hpp"
#include <SDL3_image/SDL_image.h>
#include <cmath>

Enemy::Enemy(Player* player, Map* map, Camera* camera, SDL_Renderer* renderer) // Konstrutkor przeciwnika, gracz, mapa, kamera, renderer
	: x(0), y(0), velocityX(0), velocityY(0), speed(150.0f), playerTexture(nullptr),
	frameWidth(0), frameHeight(0), currentFrame(0), currentRow(0),
	totalFrames(9), lastFrameTime(0), frameDuration(100), health(100), isAlive(true), player(player), map(map), camera(camera)
{
	LoadTexture(renderer, "spritesheet.png");
} // Przekazywanie pr�dko�ci, tekstury, klatek, czasu trwania klatki, zdrowia

Enemy::~Enemy() // Destruktor
{
	if (playerTexture) // Je�li tekstura gracza istnieje
	{
		SDL_DestroyTexture(playerTexture); // Usu� tekstur� gracza
		playerTexture = nullptr; // Ustaw tekstur� gracza na nullptr
	}

	isAlive = false; // Ustawienie na false
}

void Enemy::LoadTexture(SDL_Renderer* renderer, const char* pathFile) // Za�adowanie tekstury przeciwnika
{
	SDL_Surface* surface = IMG_Load(pathFile); // Wczytanie tekstury przeciwnika
	if (!surface) // Je�li tekstura nie istnieje
	{
		SDL_Log("Couldn't load enemy texture: %s", SDL_GetError()); // Wy�wietlenie b��du
		return; // Zwr��
	}

	playerTexture = SDL_CreateTextureFromSurface(renderer, surface); // Utworzenie tekstury przeciwnika
	frameWidth = surface->w / 9; // Szeroko�� klatki
	frameHeight = surface->h / 4; // Wysoko�� klatki
	totalFrames = 4; // Ca�kowita liczba klatek

	SDL_DestroySurface(surface); // Usuni�cie powierzchni
}

void Enemy::Render(SDL_Renderer* renderer) // Renderowanie przeciwnika
{
	if (!isAlive) return; // Je�li nie �yje

	if (camera == nullptr) // Je�li kamera nie istnieje
	{
		SDL_Log("Enemy::Render() - camera is nullptr");
		return;
	}

	renderHealthBar(health, renderer); // Renderowanie paska zdrowia

	SDL_FRect srcRect = { currentFrame * frameWidth, currentRow * frameHeight, frameWidth, frameHeight }; // Ustawienie klatki
	SDL_FRect dstRect = { x - camera->GetX() + (playerW / 16), y - camera->GetY() + (playerH / 16), enemyW, enemyH }; // Ustawienie pozycji
	SDL_RenderTexture(renderer, playerTexture, &srcRect, &dstRect); // Renderowanie tekstury
}

void Enemy::SetPosition(float x, float y) // Ustawienie pozycji przeciwnika
{
	this->x = x; // Ustawienie pozycji x
	this->y = y; // Ustawienie pozycji y
}

void Enemy::Update(float deltaTime) // Aktualizacja przeciwnika
{
	if (player == nullptr) // Je�li gracz nie istnieje
	{
		SDL_Log("Enemy::Update() - player is nullptr");
		return;
	}

	if (map == nullptr) // Je�li mapa nie istnieje
	{
		SDL_Log("Enemy::Update() - map is nullptr");
		return;
	}

	float targetX = player->GetX() - playerW / 2;	// Pozycja x gracza
	float targetY = player->GetY() - playerH / 2; // Pozycja y gracza

	float directionX = targetX - x; // Kierunek x
	float directionY = targetY - y; // Kierunek y
	float length = std::sqrt(directionX * directionX + directionY * directionY); // D�ugo��

	if (length != 0) // Je�li d�ugo�� nie jest r�wna 0
	{
		directionX /= length; // Podziel kierunek x przez d�ugo��
		directionY /= length; // Podziel kierunek y przez d�ugo��
	}

	float velocityX = directionX * speed; // Pr�dko�� x
	float velocityY = directionY * speed; // Pr�dko�� y

	float newX = x + velocityX * deltaTime; // Nowa pozycja x
	float newY = y + velocityY * deltaTime; // Nowa pozycja y

	if (velocityX == 0 && velocityY == 0) // Je�li pr�dko�� x i y s� r�wne 0
	{
		currentFrame = 0; // Ustaw klatk� na 0
	}
	else if (velocityX > 0) // Right
	{
		currentRow = 3; // Ustawk klatk� na 3
	}
	else if (velocityX < 0) // Left
	{
		currentRow = 1; // Ustaw klatk� na 1
	}
	else if (velocityY > 0) // Up
	{
		currentRow = 2; // Ustaw klatk� na 2
	}
	else if (velocityY < 0) // Down
	{
		currentRow = 0; // Ustaw klatk� na 0
	}

	x = newX; // Ustaw pozycj� x
	y = newY; // Ustaw pozycj� y

	UpdateAnimation(); // Aktualizacja animacji
}

void Enemy::renderHealthBar(double healthValue, SDL_Renderer* renderer) // Renderowanie paska 
{
	float barHeight = 5.0f; // Wysoko�� paska
	float barWidth = enemyW / 2; // Szeroko�� paska

	float healthPercentage = healthValue / 100.0f; // Procent zdrowia
	float currentBarWidth = barWidth * healthPercentage; // Aktualna szeroko�� paska

	float renderX = x - camera->GetX() + (playerW - 32.0f);
	float renderY = y - camera->GetY() + (playerH / 2);

	SDL_FRect backgroundBar = { renderX, renderY, barWidth, barHeight }; // Prostok�t t�a
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // kolor szary
	SDL_RenderFillRect(renderer, &backgroundBar); // Wype�nienie prostok�ta

	SDL_FRect foregroundBar = { renderX, renderY, currentBarWidth, barHeight }; // Prostok�t paska
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Kolor czerwony
	SDL_RenderFillRect(renderer, &foregroundBar); // Wype�nienie prostok�ta
}


void Enemy::SetVelocity(float x, float y) // Ustawienie pr�dko�ci przeciwnika
{
	velocityX = x; // Ustaw pr�dko�� x
	velocityY = y; // Ustaw pr�dko�� y
}

float Enemy::GetX() const // Pobranie pozycji x
{
	return x;
}

float Enemy::GetY() const // Pobranie pozycji y
{
	return y;
}

float Enemy::GetVelocityX() const // Pobranie pr�dko�ci x
{
	return velocityX;
}

float Enemy::GetVelocityY() const // Pobranie pr�dko�ci y
{
	return velocityY;
}

void Enemy::SetCurrentRow(int row) // Ustawienie aktualnego wiersza dla animacji
{
	currentRow = row;
}

void Enemy::UpdateAnimation() // Aktualizacja animacji
{
	if (velocityX == 0 && velocityY == 0) // Je�li pr�dko�� x i y s� r�wne 0
	{
		return;
	}

	Uint32 currentTime = SDL_GetTicks(); // Aktualny czas
	if (currentTime > lastFrameTime + frameDuration) // Je�li aktualny czas jest wi�kszy od ostatniego czasu + czasu trwania klatki
	{
		currentFrame = (currentFrame + 1) % totalFrames; // Ustaw klatk�
		lastFrameTime = currentTime; // Ustaw ostatni czas
	}
}

SDL_FRect Enemy::GetCollisionRect() const // Pobranie prostok�ta kolizji
{
	if (!isAlive)
	{
		return { 0,0,0,0 };
	}

	else
	{
		return SDL_FRect
		{
			x,
			y,
			enemyW / 2,
			enemyH / 2
		}; // Zwr�� prostok�t kolizji
	}
}