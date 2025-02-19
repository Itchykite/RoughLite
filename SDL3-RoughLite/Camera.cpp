// Camaera odopawiada za poruszanie siê ekranu za graczem, w odpowiednim zakresie

#include "Camera.hpp"

Camera::Camera(float x, float y, float width, float height) // Konstruktor kamery, pozycja x,y oraz d³ugoœæ i szerokoœæ
    : x(x), y(y), width(width), height(height), thresholdX(width / 4), thresholdY(height / 4) 
{
}

void Camera::Update(float playerX, float playerY)
{
	if (playerX > x + width - thresholdX) // Jeœli gracz przekroczy prawy próg kamery
    {
		x = playerX - (width - thresholdX); // Przesuñ kamerê w prawo
    }
	else if (playerX < x + thresholdX) // Jeœli gracz przekroczy lewy próg kamery
    {
		x = playerX - thresholdX; // Przesuñ kamerê w lewo
    }

	if (playerY > y + height - thresholdY) // Jeœli gracz przekroczy dolny próg kamery
    {
		y = playerY - (height - thresholdY); // Przesuñ kamerê w dó³
    }
	else if (playerY < y + thresholdY) // Jeœli gracz przekroczy górny próg kamery
    {
		y = playerY - thresholdY; // Przesuñ kamerê w górê
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
