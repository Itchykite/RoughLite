#pragma once

class Camera
{
public:
    Camera(float x, float y, float width, float height); // Konstruktor
    void Update(float playerX, float playerY); // Aktualizacja pozycji kamery
    float GetX() const; // pobierz x
    float GetY() const; // pobierz y

    float width, height; // szeroko�� i wysoko�� ekranu
    float thresholdX, thresholdY; // pr�g zakresu kamery

private:
    float x, y; // x y
};