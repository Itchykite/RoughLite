#include "Enemy.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include <SDL3_image/SDL_image.h>
#include <cmath>

float Enemy::playerW = 256.0f;
float Enemy::playerH = 256.0f;

float Enemy::enemyW = 128.0f;
float Enemy::enemyH = 128.0f;

Enemy::Enemy(Player* player, Map* map, Camera* camera, SDL_Renderer* renderer)
	: x(0), y(0), velocityX(0), velocityY(0), speed(150.0f), playerTexture(nullptr),
	frameWidth(0), frameHeight(0), currentFrame(0), currentRow(0),
	totalFrames(9), lastFrameTime(0), frameDuration(100), health(100), player(player), map(map), camera(camera)
{
	LoadTexture(renderer, "spritesheet.png");
}

Enemy::~Enemy()
{
	if (playerTexture)
	{
		SDL_DestroyTexture(playerTexture);
		playerTexture = nullptr;
	}
}

void Enemy::LoadTexture(SDL_Renderer* renderer, const char* pathFile)
{
	SDL_Surface* surface = IMG_Load(pathFile);
	if (!surface)
	{
		SDL_Log("Couldn't load enemy texture: %s", SDL_GetError());
		return;
	}

	playerTexture = SDL_CreateTextureFromSurface(renderer, surface);
	frameWidth = surface->w / 9;
	frameHeight = surface->h / 4;
	totalFrames = 4;

	SDL_DestroySurface(surface);
}

void Enemy::Render(SDL_Renderer* renderer)
{
	if (camera == nullptr)
	{
		SDL_Log("Enemy::Render() - camera is nullptr");
		return;
	}

	SDL_FRect srcRect = { currentFrame * frameWidth, currentRow * frameHeight, frameWidth, frameHeight };
	SDL_FRect dstRect = { x - camera->GetX() + (playerW / 2), y - camera->GetY() + (playerH / 2), enemyW, enemyH };
	SDL_RenderTexture(renderer, playerTexture, &srcRect, &dstRect);
}

void Enemy::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Enemy::Update(float deltaTime)
{
	if (player == nullptr)
	{
		SDL_Log("Enemy::Update() - player is nullptr");
		return;
	}

	if (map == nullptr)
	{
		SDL_Log("Enemy::Update() - map is nullptr");
		return;
	}

	float targetX = player->GetX();
	float targetY = player->GetY();

	float directionX = targetX - x;
	float directionY = targetY - y;
	float length = std::sqrt(directionX * directionX + directionY * directionY);

	if (length != 0)
	{
		directionX /= length;
		directionY /= length;
	}

	float velocityX = directionX * speed;
	float velocityY = directionY * speed;

	float newX = x + velocityX * deltaTime;
	float newY = y + velocityY * deltaTime;

	if (velocityX == 0 && velocityY == 0)
	{
		currentFrame = 0;
	}
	else if (velocityX > 0) // Right
	{
		currentRow = 3;
	}
	else if (velocityX < 0) // Left
	{
		currentRow = 1;
	}
	else if (velocityY > 0) // Up
	{
		currentRow = 2;
	}
	else if (velocityY < 0) // Down
	{
		currentRow = 0;
	}

	x = newX;
	y = newY;

	UpdateAnimation();
}

void Enemy::SetVelocity(float x, float y)
{
	velocityX = x;
	velocityY = y;
}

float Enemy::GetX() const
{
	return x;
}

float Enemy::GetY() const
{
	return y;
}

float Enemy::GetVelocityX() const
{
	return velocityX;
}

float Enemy::GetVelocityY() const
{
	return velocityY;
}

void Enemy::SetCurrentRow(int row)
{
	currentRow = row;
}

void Enemy::UpdateAnimation()
{
	if (velocityX == 0 && velocityY == 0)
	{
		return;
	}

	Uint32 currentTime = SDL_GetTicks();
	if (currentTime > lastFrameTime + frameDuration)
	{
		currentFrame = (currentFrame + 1) % totalFrames;
		lastFrameTime = currentTime;
	}
}

SDL_FRect Enemy::GetCollisionRect() const
{
	return SDL_FRect
	{
		x,
		y,
		enemyW / 2,
		enemyH / 2
	};
}