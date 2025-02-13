#include "Camera.hpp"

Camera::Camera(float x, float y, float width, float height)
    : x(x), y(y), width(width), height(height), thresholdX(width / 4), thresholdY(height / 4)
{
}

void Camera::Update(float playerX, float playerY)
{
    if (playerX > x + width - thresholdX)
    {
        x = playerX - (width - thresholdX);
    }
    else if (playerX < x + thresholdX)
    {
        x = playerX - thresholdX;
    }

    if (playerY > y + height - thresholdY)
    {
        y = playerY - (height - thresholdY);
    }
    else if (playerY < y + thresholdY)
    {
        y = playerY - thresholdY;
    }
}

float Camera::GetX() const
{
    return x;
}

float Camera::GetY() const
{
    return y;
}
