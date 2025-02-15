#include "Enemy.hpp"
#include "Player.hpp"
#include <SDL3_image/SDL_image.h>
#include <cmath>

float Enemy::playerW = 128.0f;
float Enemy::playerH = 128.0f;

Enemy::Enemy(Player* player, SDL_Renderer* renderer)
	: x(0), y(0), velocityX(0), velocityY(0), playerTexture(nullptr),
	frameWidth(0), frameHeight(0), currentFrame(0), currentRow(0),
	totalFrames(0), lastFrameTime(0), frameDuration(100), health(100), player(player)
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
	if (surface == nullptr)
	{
		SDL_Log("Failed to load player texture: %s", SDL_GetError());
		return;
	}

	playerTexture = SDL_CreateTextureFromSurface(renderer, surface);
	if (playerTexture == nullptr)
	{
		SDL_Log("Failed to create player texture: %s", SDL_GetError());
		SDL_DestroySurface(surface);
		return;
	}

	frameWidth = surface->w / 9;
	frameHeight = surface->h / 4;
	SDL_DestroySurface(surface);
}

void Enemy::Render(SDL_Renderer* renderer)
{
	SDL_FRect srcRect = { currentFrame * frameWidth, currentRow * frameHeight, frameWidth, frameHeight };
	SDL_FRect dstRect = { static_cast<int>(x), static_cast<int>(y), frameWidth, frameHeight };
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

	double distanceX = player->GetX() - x;
	double distanceY = player->GetY() - y;

	if (distanceX > 0)
	{
		x += speed * deltaTime;
	}

	else if (distanceX < 0)
	{
		x -= speed * deltaTime;
	}

	if (distanceY > 0)
	{
		y += speed * deltaTime;
	}

	else if (distanceY < 0)
	{
		y -= speed * deltaTime;
	}
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