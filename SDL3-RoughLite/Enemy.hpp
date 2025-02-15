#pragma once

#include "Player.hpp"
#include <SDL3/SDL.h>

class Enemy
{
public:
	Enemy(Player* player ,SDL_Renderer* renderer);
	~Enemy();

	void LoadTexture(SDL_Renderer* renderer, const char* pathFile);
	void Render(SDL_Renderer* renderer);
	void SetPosition(float x, float y);
	void Update();
	void SetVelocity(float x, float y);
	float GetX() const;
	float GetY() const;
	float GetVelocityX() const;
	float GetVelocityY() const;
	void SetCurrentRow(int row);

	static float playerW;
	static float playerH;

private:
	Player* player;

	void UpdateAnimation();

	float x;
	float y;
	float velocityX;
	float velocityY;
	float directionX;
	float directionY;
	float speed = 10.0f;
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