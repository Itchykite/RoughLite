#include "Map.hpp"
#include "Player.hpp"

Map::Map(int width, int height) : width(width), height(height)
{

}

bool Map::IsWithinBounds(float x, float y, float px, float py) const
{
    return x >= 0 && x <= (width - px) && y >= 0 && y <= (height - py);
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