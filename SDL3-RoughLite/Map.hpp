#pragma once

// Map odpowiada za map�, t�o, kolizje, siatk�

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <utility>
#include "Settings.hpp"

class Player;

struct ObjectCollision 
{
	SDL_Texture* texture;
	SDL_FRect rect;
	std::vector<SDL_FRect> collisionRects;
};

class Map
{
public:
	Map(int width, int height); // Konstruktor mapy, szeroko��, wysoko��
	bool IsWithinBounds(float x, float y, float px, float py) const; // Czy jest w granicach
	bool LoadTexture(SDL_Renderer* renderer, const char* pathFile); // Wczytanie tekstury
	void Render(SDL_Renderer* renderer, float cameraX, float cameraY) const; // Renderowanie
	void RenderObjects(SDL_Renderer* renderer, float cameraX, float CameraY, Player* player) const;
	bool IsNodeWalkable(int x, int y) const; // Czy w�ze� jest dost�pny
	void CreateGrid(int nodeSize); // Utworzenie siatki
	void AddCollisionRect(const SDL_FRect& rect); // Dodanie prostok�ta kolizji
	bool LoadCollisionSurface(const char* pathFile); // Wczytanie powierzchni kolizji
	bool IsPixelTransparent(int x, int y) const; // Czy piksel jest przezroczysty
	void CreateCollisionSurface(); // Utworzenie powierzchni kolizji
	void AddObjectWithCollision(SDL_Renderer* renderer, const char* objectTexturePath, float posX, float posY, float objWidth, float objHeight);
	void SortObjectsByY();
	void Reset(SDL_Renderer* renderer);

	SDL_Surface* collisionSurface; // Powierzchnia kolizji
	std::vector<ObjectCollision> objectCollisions;

private:
	int width; // Szeroko��
	int height; // Wysoko��
	int nodeSize; // Rozmiar w�z�a
	SDL_Texture* backgroundTexture; // Tekstura t�a
	std::vector<std::vector<bool>> grid; // Siatka
	std::vector<SDL_FRect> collisionRects; // Prostok�ty kolizji
	std::vector<std::pair<SDL_Texture*, SDL_FRect>> objectsWithCollision;
};

void mapCollisions(Map* map, SDL_Renderer* renderer); // Kolizje mapy