#include "RangeRover.hpp"
#include "Map.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include <SDL3_image/SDL_image.h>
#include <cmath>

float RangeRover::numOfRangeRovers = 0;

RangeRover::RangeRover(Player* player, Map* map, Camera* camera, SDL_Renderer* renderer)
	: x(0), y(0), velocityX(0), velocityY(0), speed(10.0f), playerTexture(nullptr),
	frameWidth(0), frameHeight(0), currentFrame(0), currentRow(0),
	totalFrames(9), lastFrameTime(0), frameDuration(100), player(player), map(map), camera(camera), 
	angle(0.0f), radius(200.0f)
{
	LoadTexture(renderer, "spritesheet.png");
}

RangeRover::~RangeRover()
{
	if (playerTexture)
	{
		SDL_DestroyTexture(playerTexture);
		playerTexture = nullptr;
	}
}

void RangeRover::LoadTexture(SDL_Renderer* renderer, const char* pathFile)
{
	SDL_Surface* surface = IMG_Load(pathFile);
	if (!surface)
	{
		SDL_Log("Couldn't load range rover texture: %s", SDL_GetError());
		return;
	}

	playerTexture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!playerTexture)
	{
		SDL_Log("Couldn't create texture from surface: %s", SDL_GetError());
	}

	frameWidth = surface->w / 9;
	frameHeight = surface->h / 4;
	totalFrames = 4;

	SDL_DestroySurface(surface);
}

void RangeRover::Render(SDL_Renderer* renderer)
{
	if (camera == nullptr)
	{
		SDL_Log("Enemy::Render() - camera is nullptr");
		return;
	}

	SDL_FRect srcRect = { currentFrame * frameWidth, currentRow * frameHeight, frameWidth, frameHeight };
	SDL_FRect dstRect = { x - camera->GetX() + (Enemy::playerW / 2), y - camera->GetY() + (Enemy::playerH / 2), Enemy::enemyW, Enemy::enemyH };
	SDL_RenderTexture(renderer, playerTexture, &srcRect, &dstRect);
}

void RangeRover::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void RangeRover::Update(float deltaTime)
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

	angle += speed * deltaTime;
	if (angle > 360.0f)
	{
		angle -= 360.0f;
	}

	x = targetX + radius * std::cos(angle);
	y = targetY + radius * std::sin(angle);

	UpdateAnimation();
}

void RangeRover::SetVelocity(float x, float y)
{
	velocityX = x;
	velocityY = y;
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

void RangeRover::UpdateAnimation()
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

SDL_FRect RangeRover::GetCollisionRect() const
{
	return SDL_FRect
	{
		x,
		y,
		Enemy::enemyW / 2,
		Enemy::enemyH / 2
	};
}