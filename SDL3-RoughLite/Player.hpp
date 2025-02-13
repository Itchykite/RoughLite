#pragma once
#include <SDL3/SDL.h>
#include "Map.hpp"
#include "Camera.hpp"

class Player
{
public:
	Player(Map* map, Camera* camera);
	~Player();

	void Render(SDL_Renderer* renderer);
	void SetPosition(float x, float y);
	void Update();
	void SetVelocity(float x, float y);
	float GetX() const;
	float GetY() const;
	float GetVelocityX() const;
	float GetVelocityY() const;

	static float playerW;
	static float playerH;

private:
	float x;
	float y;
	float velocityX;
	float velocityY;
	Map* map;
	Camera* camera;
};