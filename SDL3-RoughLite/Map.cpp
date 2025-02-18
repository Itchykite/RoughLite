#include "Map.hpp"
#include "Player.hpp"

Map::Map(int width, int height) : width(width), height(height)
{
    CreateCollisionSurface();
}

void Map::CreateCollisionSurface()
{
    collisionSurface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
    if (!collisionSurface)
    {
        SDL_Log("Unable to create collision surface! SDL Error: %s", SDL_GetError());
    }
}

bool Map::IsWithinBounds(float x, float y, float px, float py) const
{
    if (x < 64.0f || x >(width - px - 64.0f) || y < 0.0f || y >(height - py - 63.0f))
    {
        return false;
    }

    SDL_FRect playerRect = { x, y, px, py };
    for (const auto& rect : collisionRects)
    {
        if (SDL_HasRectIntersectionFloat(&playerRect, &rect))
        {
            return false;
        }
    }

    return true;
}

bool Map::LoadTexture(SDL_Renderer* renderer, const char* pathFile)
{
    backgroundTexture = IMG_LoadTexture(renderer, pathFile);
    return backgroundTexture != nullptr;
}

void Map::Render(SDL_Renderer* renderer, float cameraX, float cameraY) const
{
    if (backgroundTexture)
    {
        SDL_FRect srcRect = { 0, 0, width, height };
        SDL_FRect dstRect = { -cameraX, -cameraY, width, height };
        SDL_RenderTexture(renderer, backgroundTexture, &srcRect, &dstRect);
    }
}

bool Map::IsNodeWalkable(int x, int y) const
{
    if (x < 0 || x >= width / nodeSize || y < 0 || y >= height / nodeSize)
    {
        return false;
    }

    return grid[y][x];
}

void Map::CreateGrid(int nodeSize)
{
    this->nodeSize = nodeSize;
    int rows = height / nodeSize;
    int cols = width / nodeSize;
    grid.resize(rows, std::vector<bool>(cols, true));
}

void Map::AddCollisionRect(const SDL_FRect& rect)
{
    collisionRects.push_back(rect);
}

bool Map::LoadCollisionSurface(const char* pathFile)
{
    SDL_Surface* loadedSurface = IMG_Load(pathFile);
    if (!loadedSurface)
    {
        SDL_Log("Unable to load collision surface %s! SDL_image Error: %s", pathFile, SDL_GetError());
        return false;
    }

    SDL_Surface* scaledSurface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
    if (!scaledSurface)
    {
        SDL_Log("Unable to create scaled surface! SDL Error: %s", SDL_GetError());
        SDL_DestroySurface(loadedSurface);
        return false;
    }

    SDL_BlitSurfaceScaled(loadedSurface, nullptr, scaledSurface, nullptr, SDL_SCALEMODE_NEAREST);
    SDL_DestroySurface(loadedSurface);

    if (collisionSurface)
    {
        SDL_DestroySurface(collisionSurface);
    }
    collisionSurface = scaledSurface;

    return true;
}

bool Map::IsPixelTransparent(int x, int y) const
{
    y += 128;
    x += 0;

    if (!collisionSurface || x < 0 || x >= collisionSurface->w || y < 0 || y >= collisionSurface->h)
    {
        return true;
    }

    Uint32* pixels = (Uint32*)collisionSurface->pixels;
    Uint32 pixel = pixels[(y * collisionSurface->w) + x];
    

    Uint8 r, g, b, a;
	SDL_GetRGBA(pixel, SDL_GetPixelFormatDetails(collisionSurface->format), 0, &r, &g, &b, &a);

    return a == 0;
}

void mapCollisions(Map* map)
{
	//map->AddCollisionRect({ 512, 512, 64, 64 });
}