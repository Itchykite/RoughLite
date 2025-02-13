#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

class Map
{
public:
    Map(int width, int height);
    bool IsWithinBounds(float x, float y, float px, float py) const;
    bool LoadTexture(SDL_Renderer* renderer, const char* pathFile);
    void Render(SDL_Renderer* renderer, float cameraX, float cameraY) const;

private:
    int width;
    int height;
    SDL_Texture* backgroundTexture;
};