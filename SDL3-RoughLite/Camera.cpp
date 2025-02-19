// Camaera odopawiada za poruszanie si� ekranu za graczem, w odpowiednim zakresie

#include "Camera.hpp"

Camera::Camera(float x, float y, float width, float height) // Konstruktor kamery, pozycja x,y oraz d�ugo�� i szeroko��
    : x(x), y(y), width(width), height(height), thresholdX(width / 4), thresholdY(height / 4) 
{
}

void Camera::Update(float playerX, float playerY)
{
	if (playerX > x + width - thresholdX) // Je�li gracz przekroczy prawy pr�g kamery
    {
		x = playerX - (width - thresholdX); // Przesu� kamer� w prawo
    }
	else if (playerX < x + thresholdX) // Je�li gracz przekroczy lewy pr�g kamery
    {
		x = playerX - thresholdX; // Przesu� kamer� w lewo
    }

	if (playerY > y + height - thresholdY) // Je�li gracz przekroczy dolny pr�g kamery
    {
		y = playerY - (height - thresholdY); // Przesu� kamer� w d�
    }
	else if (playerY < y + thresholdY) // Je�li gracz przekroczy g�rny pr�g kamery
    {
		y = playerY - thresholdY; // Przesu� kamer� w g�r�
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
