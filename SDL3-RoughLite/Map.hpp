#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>

class Map
{
public:
    Map(int width, int height);
    bool IsWithinBounds(float x, float y, float px, float py) const;
    bool LoadTexture(SDL_Renderer* renderer, const char* pathFile);
    void Render(SDL_Renderer* renderer, float cameraX, float cameraY) const;
	bool IsNodeWalkable(int x, int y) const;
	void CreateGrid(int nodeSize);
    void AddCollisionRect(const SDL_FRect& rect);
    bool LoadCollisionSurface(const char* pathFile);
    bool IsPixelTransparent(int x, int y) const;
    void CreateCollisionSurface();

    SDL_Surface* collisionSurface;

private:
    int width;
    int height;
    int nodeSize;
    SDL_Texture* backgroundTexture;
    std::vector<std::vector<bool>> grid;
    std::vector<SDL_FRect> collisionRects;
};

void mapCollisions(Map* map);