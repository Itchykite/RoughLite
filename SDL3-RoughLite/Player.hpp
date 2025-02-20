#pragma once

// Player odpowiada za gracza, jego zachowanie, wygl�d, kolizje

#include <SDL3/SDL.h>
#include "Map.hpp"
#include "Camera.hpp"
#include <memory>
#include <vector>

class Enemy;

class Player
{
public:
    Player(Map* map, Camera* camera, SDL_Renderer* renderer); // Konstruktor gracza, mapa, kamera, renderer
    ~Player(); // Destruktor

    void LoadTexture(SDL_Renderer* renderer, const char* pathFile); // Za�adowanie tekstury gracza
    void Render(SDL_Renderer* renderer); // Renderowanie gracza
    void SetPosition(float x, float y); // Ustwienie pozycji gracza
    void Update(float deltaTime); // Aktualizacja gracza
    void SetVelocity(float x, float y); // Ustwienie pr�dko�ci gracza
    float GetX() const; // Pobranie pozycji x 
    float GetY() const; // Pobranie pozycji y
    float GetVelocityX() const; // Pobranie pr�dko�ci x
    float GetVelocityY() const; // Pobranie pr�dko�ci y
    void SetCurrentRow(int row); // Ustwienie aktualnego wiersza dla animacji
    void HandleCollision(); // Obs�uga kolizji
    void attack(std::vector<std::unique_ptr<Enemy>>& enemies, float dirX, float dirY); // Atak

    static float playerW; // Szeroko�� gracza
    static float playerH; // Wysoko�� gracza
    long int kills; // Wynik

    Map* map;

private:
    void UpdateAnimation(); // Aktualizacja animacji
    void renderHealthBar(double healthValue, SDL_Renderer* renderer); // Renderowanie paska zdrowia

    float x; // Pozycja x
    float y; // Pozycja y
    float velocityX; // Pr�dko�� x
    float velocityY; // Pr�dko�� y
    Camera* camera; // Kamera

    SDL_Texture* playerTexture; // Tekstura gracza
    int frameWidth; // Szeroko�� klatki
    int frameHeight; // Wysoko�� klatki
    int currentFrame; // Aktualna klatka
    int currentRow; // Aktualny wiersz
    int totalFrames; // Ca�kowita liczba klatek
    Uint32 lastFrameTime; // Ostatni czas klatki
    Uint32 frameDuration; // Czas trwania klatki

    double health; // Zdrowie
};