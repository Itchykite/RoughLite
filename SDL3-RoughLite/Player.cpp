#include "Player.hpp"

float Player::playerW = 10.0f;
float Player::playerH = 10.0f;

Player::Player(Map* map, Camera* camera) : x(0), y(0), velocityX(0), velocityY(0), map(map), camera(camera)
{
}

Player::~Player()
{
}

void Player::Update()
{
    float newX = x + velocityX;
    float newY = y + velocityY;

    if (map->IsWithinBounds(newX, newY, playerW, playerH))
    {
        x = newX;
        y = newY;
    }

    camera->Update(x, y);
}

void Player::Render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_FRect player = { x - camera->GetX(), y - camera->GetY(), 10.0f, 10.0f };
    SDL_RenderFillRect(renderer, &player);
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
