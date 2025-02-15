#pragma once

#include "Player.hpp"
#include "Map.hpp"
#include "Camera.hpp"
#include "Map.hpp"
#include <SDL3/SDL.h>
#include <vector>

class Enemy
{
public:
	Enemy(Player* player, Map* map, Camera* camera, SDL_Renderer* renderer);
	~Enemy();

	void LoadTexture(SDL_Renderer* renderer, const char* pathFile);
	void Render(SDL_Renderer* renderer);
	void SetPosition(float x, float y);
	void Update(float deltaTime);
	void SetVelocity(float x, float y);
	float GetX() const;
	float GetY() const;
	float GetVelocityX() const;
	float GetVelocityY() const;
	void SetCurrentRow(int row);

	SDL_FRect GetCollisionRect() const;

	static float playerW;
	static float playerH;

	static float enemyW;
	static float enemyH;

private:
	Player* player;
	Map* map;
	Camera* camera;

	void UpdateAnimation();

	float x;
	float y;
	float velocityX;
	float velocityY;
	float directionX;
	float directionY;
	float speed;
	SDL_Texture* playerTexture;
	int frameWidth;
	int frameHeight;
	int currentFrame;
	int currentRow;
	int totalFrames;
	Uint32 lastFrameTime;
	Uint32 frameDuration;

	double health;
};