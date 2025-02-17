#pragma once
#include <SDL3/SDL.h>
#include "Map.hpp"
#include "Camera.hpp"

class Player
{
public:
	Player(Map* map, Camera* camera, SDL_Renderer* renderer);
	~Player();

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
	void HandleCollision();

	static float playerW;
	static float playerH;

	Map* map;

private:
	void UpdateAnimation();

	float x;
	float y;
	float velocityX;
	float velocityY;
	Camera* camera;

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