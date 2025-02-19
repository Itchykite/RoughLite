#include "Player.hpp"
#include <SDL3_image/SDL_image.h>

float Player::playerW = 128.0f;
float Player::playerH = 128.0f;

Player::Player(Map* map, Camera* camera, SDL_Renderer* renderer) // Konstruktor gracza, mapa, kamera, renderer
    : map(map), camera(camera), x(0), y(0), velocityX(0), velocityY(0),
    playerTexture(nullptr), frameWidth(0), frameHeight(0), currentFrame(0),
    totalFrames(0), lastFrameTime(0), frameDuration(100), currentRow(0)
{
    SDL_Log("Loading player texture...");
	LoadTexture(renderer, "spritesheet.png"); // Wczytanie tekstury gracza
}

Player::~Player() // Destruktor
{
	if (playerTexture) // Je�li tekstura gracza istnieje
    {
        SDL_Log("Destroying player texture...");
		SDL_DestroyTexture(playerTexture); // Usuni�cie tekstury gracza
		playerTexture = nullptr; // Ustawienie tekstury gracza na nullptr
    }
}

void Player::HandleCollision() // Obs�uga kolizji
{
	if (!map->IsWithinBounds(x, y, 64, 64)) // Je�li gracz nie jest w granicach
    {
		x -= velocityX; // Ustawienie pozycji x
		y -= velocityY; // Ustawienie pozycji y
    }
}

void Player::Update(float deltaTime) // Aktualizacja gracza
{ 
	float newX = x + velocityX * deltaTime; // Nowa pozycja x
	float newY = y + velocityY * deltaTime; // Nowa pozycja y

	if (map->IsWithinBounds(newX, newY, playerW, playerH)) // Je�li gracz jest w granicach
    {
		x = newX; // Ustaw pozycj� x
		y = newY; // Ustaw pozycj� y
    }

	if (velocityX == 0 && velocityY == 0) // Je�li pr�dko�� x i y s� r�wne 0
    {
		currentFrame = 0; // Ustaw klatk� na 0
    }
	else if (velocityX > 0) // Right
    {
		currentRow = 3; // Ustaw klatk� na 3
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

	if (camera != nullptr) // Je�li kamera istnieje
    {
		camera->Update(x, y); // Aktualizacja kamery
    }
    else
    {
        SDL_Log("Camera is not initialized!");
    }

	UpdateAnimation(); // Aktualizacja animacji
}

void Player::SetCurrentRow(int row) // Ustawienie aktualnego wiersza dla animacji
{
	currentRow = row; // Ustaw wiersz
}

void Player::LoadTexture(SDL_Renderer* renderer, const char* pathFile) // Wczytanie tekstury gracza
{
	SDL_Surface* surface = IMG_Load(pathFile); // Wczytanie powierzchni gracza
	if (!surface) // Je�li powierzchnia gracza nie istnieje
    {
        SDL_Log("Couldn't load player texture: %s", SDL_GetError());
        return;
    }

	playerTexture = SDL_CreateTextureFromSurface(renderer, surface); // Utworzenie tekstury gracza
	frameWidth = surface->w / 9; // Szeroko�� klatki
	frameHeight = surface->h / 4; // Wysoko�� klatki
	totalFrames = 4; // Ca�kowita liczba klatek

	SDL_DestroySurface(surface); // Usuni�cie powierzchni
}

void Player::UpdateAnimation() // Aktualizacja animacji
{
	if (velocityX == 0 && velocityY == 0) // Je�li pr�dko�� x i y s� r�wne 0
    {
        return;
    }

	Uint32 currentTime = SDL_GetTicks(); // Aktualny czas
	if (currentTime > lastFrameTime + frameDuration) // Je�eli aktualny czas jest wi�kszy od ostatniego czasu klatki + czasu trwania klatki
    {
		currentFrame = (currentFrame + 1) % totalFrames; // Ustaw klatk�
		lastFrameTime = currentTime; // Ustaw ostatni czas klatki
    }
}

void Player::Render(SDL_Renderer* renderer) // Renderowanie gracza
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Ustaw kolor renderowania
	SDL_FRect player = { x - camera->GetX(), y - camera->GetY(), playerW, playerH }; // Ustawienie pozycji gracza
	SDL_FRect srcRect = { static_cast<int>(currentFrame * frameWidth), static_cast<int>(currentRow * frameHeight), static_cast<int>(frameWidth), static_cast<int>(frameHeight) }; // Ustawienie klatki
	SDL_RenderTexture(renderer, playerTexture, &srcRect, &player); // Renderowanie tekstury
}

void Player::SetPosition(float x, float y) 
{
	this->x = x; // Ustaw pozycj� x
	this->y = y; // Ustaw pozycj� y
}

void Player::SetVelocity(float x, float y)
{
	this->velocityX = x; // Ustaw pr�dko�� x
	this->velocityY = y; // Ustaw pr�dko�� y
}

float Player::GetX() const
{
    return x;
}

float Player::GetY() const
{
    return y;
}

float Player::GetVelocityX() const
{
    return velocityX;
}

float Player::GetVelocityY() const
{
    return velocityY;
}
