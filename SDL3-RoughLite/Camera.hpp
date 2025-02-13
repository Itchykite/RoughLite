#pragma once

class Camera
{
public:
    Camera(float x, float y, float width, float height);
    void Update(float playerX, float playerY);
    float GetX() const;
    float GetY() const;

private:
    float x, y;
    float width, height;
    float thresholdX, thresholdY;
};