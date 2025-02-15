#include "Player.hpp"
#include <SDL3_image/SDL_image.h>

float Player::playerW = 128.0f;
float Player::playerH = 128.0f;

Player::Player(Map* map, Camera* camera, SDL_Renderer* renderer)
    : map(map), camera(camera), x(0), y(0), velocityX(0), velocityY(0),
    playerTexture(nullptr), frameWidth(0), frameHeight(0), currentFrame(0),
    totalFrames(0), lastFrameTime(0), frameDuration(100), currentRow(0)
{
    SDL_Log("Loading player texture...");
    LoadTexture(renderer, "spritesheet.png");
}

Player::~Player()
{
    if (playerTexture)
    {
        SDL_Log("Destroying player texture...");
        SDL_DestroyTexture(playerTexture);
        playerTexture = nullptr;
    }
}

void Player::Update(float deltaTime)
{
    SDL_Log("Updating player position...");
    float newX = x + velocityX * deltaTime;
    float newY = y + velocityY * deltaTime;

    SDL_Log("Player position: (%f, %f)", x, y);

    if (map->IsWithinBounds(newX, newY, playerW, playerH))
    {
        x = newX;
        y = newY;
    }

    if (velocityX == 0 && velocityY == 0)
    {
        currentFrame = 0;
    }
	else if (velocityX > 0) // Right
    {
        currentRow = 3;
    }
	else if (velocityX < 0) // Left
    {
        currentRow = 1;
    }
	else if (velocityY > 0) // Up
    {
        currentRow = 2;
    }
    else if (velocityY < 0) // Down
    {
        currentRow = 0;
    }

    if (camera != nullptr)
    {
        camera->Update(x, y);
    }
    else
    {
        SDL_Log("Camera is not initialized!");
    }

    UpdateAnimation();
}

void Player::SetCurrentRow(int row)
{
    currentRow = row;
}

void Player::LoadTexture(SDL_Renderer* renderer, const char* pathFile)
{
    SDL_Surface* surface = IMG_Load(pathFile);
    if (!surface)
    {
        SDL_Log("Couldn't load player texture: %s", SDL_GetError());
        return;
    }

    playerTexture = SDL_CreateTextureFromSurface(renderer, surface);
    frameWidth = surface->w / 9;
    frameHeight = surface->h / 4;
    totalFrames = 4;

    SDL_DestroySurface(surface);
}

void Player::UpdateAnimation()
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

void Player::Render(SDL_Renderer* renderer)
{
    SDL_Log("Rendering player...");
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_FRect player = { x - camera->GetX(), y - camera->GetY(), playerW, playerH };
    SDL_FRect srcRect = { static_cast<int>(currentFrame * frameWidth), static_cast<int>(currentRow * frameHeight), static_cast<int>(frameWidth), static_cast<int>(frameHeight) };
    SDL_RenderTexture(renderer, playerTexture, &srcRect, &player);
}

void Player::SetPosition(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Player::SetVelocity(float x, float y)
{
    this->velocityX = x;
    this->velocityY = y;
}

float Player::GetX() const
{
    return x;
}

float Player::GetY() const
{
    return y;
}

float Player::GetVelocityX() const
{
    return velocityX;
}

float Player::GetVelocityY() const
{
    return velocityY;
}
