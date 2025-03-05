// Camaera odopawiada za poruszanie si� ekranu za graczem, w odpowiednim zakresie

#include "Camera.hpp"
#include "Settings.hpp"

Camera::Camera(float x, float y, float width, float height) // Konstruktor kamery, pozycja x,y oraz d�ugo�� i szeroko��
    : x(x), y(y), width(width), height(height), thresholdX(width / 4), thresholdY(height / 4) 
{
}

void Camera::Update(float playerX, float playerY)
{
	if (playerX > x + width - thresholdX - playerW / 2) // Je�li gracz przekroczy prawy pr�g kamery
    {
		x = playerX - (width - thresholdX - playerW / 2); // Przesu� kamer� w prawo
    }
	else if (playerX < x + thresholdX) // Je�li gracz przekroczy lewy pr�g kamery
    {
		x = playerX - thresholdX; // Przesu� kamer� w lewo
    }

	if (playerY > y + height - thresholdY - playerH / 2) // Je�li gracz przekroczy dolny pr�g kamery
    {
		y = playerY - (height - thresholdY - playerH / 2); // Przesu� kamer� w d�
    }
	else if (playerY < y + thresholdY) // Je�li gracz przekroczy g�rny pr�g kamery
    {
		y = playerY - thresholdY; // Przesu� kamer� w g�r�
    }

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + width > mapWidth) x = mapWidth - width;
    if (y + height > mapHeight) y = mapHeight - height;
}

float Camera::GetX() const
{
    return x;
}

float Camera::GetY() const
{
    return y;
}
