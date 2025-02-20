#pragma once

#include <SDL3/SDL.h>
#include "Map.hpp"
#include "Camera.hpp"
#include <memory>

class Player;

class Enemy
{
public:
    Enemy(Player* player, Map* map, Camera* camera, SDL_Renderer* renderer); // Konstruktor przeciwnika, gracz, mapa, kamera, renderer
    ~Enemy(); // Destruktor

    void LoadTexture(SDL_Renderer* renderer, const char* pathFile); // Za�adowanie tekstury przeciwnika
    void Render(SDL_Renderer* renderer); // Renderowanie przeciwnika
    void SetPosition(float x, float y); // Ustwienie pozycji przeciwnika
    void Update(float deltaTime); // Aktualizacja przeciwnika
    void SetVelocity(float x, float y); // Ustwienie pr�dko�ci przeciwnika
    float GetX() const; // Pobranie pozycji x
    float GetY() const; // Pobranie pozycji y
    float GetVelocityX() const; // Pobranie pr�dko�ci x
    float GetVelocityY() const; // Pobranie pr�dko�ci y
    void SetCurrentRow(int row); // Ustwienie aktualnego wiersza dla animacji

    SDL_FRect GetCollisionRect() const; // Pobranie prostok�ta kolizji

    Player* player; // Gracz
    Map* map; // Mapa
    Camera* camera; // Kamera

    double health; // Zdrowie

private:
    void UpdateAnimation(); // Aktualizacja animacji
    void renderHealthBar(double healthValue, SDL_Renderer* renderer); // Renderowanie paska zdrowia

    float x; // Pozycja x
    float y; // Pozycja y
    float velocityX; // Pr�dko�� x
    float velocityY; // Pr�dko�� y
    float directionX; // Kierunek x
    float directionY; // Kierunek y
    float speed; // Pr�dko��
    SDL_Texture* playerTexture; // Tekstura przeciwnika
	SDL_Renderer* renderer; // Renderer
    int frameWidth; // Szeroko�� klatki
    int frameHeight; // Wysoko�� klatki
    int currentFrame; // Aktualna klatka
    int currentRow; // Aktualny wiersz
    int totalFrames; // Ca�kowita liczba klatek
	bool isAlive; // Czy �yje
    Uint32 lastFrameTime; // Ostatni czas klatki
    Uint32 frameDuration; // Czas trwania klatki
};
