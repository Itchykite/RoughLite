#pragma once

// Player odpowiada za gracza, jego zachowanie, wygl¹d, kolizje

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

    void LoadTexture(SDL_Renderer* renderer, const char* pathFile); // Za³adowanie tekstury gracza
    void Render(SDL_Renderer* renderer); // Renderowanie gracza
    void SetPosition(float x, float y); // Ustwienie pozycji gracza
    void Update(float deltaTime); // Aktualizacja gracza
    void SetVelocity(float x, float y); // Ustwienie prêdkoœci gracza
    float GetX() const; // Pobranie pozycji x 
    float GetY() const; // Pobranie pozycji y
    float GetVelocityX() const; // Pobranie prêdkoœci x
    float GetVelocityY() const; // Pobranie prêdkoœci y
    void SetCurrentRow(int row); // Ustwienie aktualnego wiersza dla animacji
    void HandleCollision(); // Obs³uga kolizji
    void attack(std::vector<std::unique_ptr<Enemy>>& enemies, float dirX, float dirY); // Atak

    static float playerW; // Szerokoœæ gracza
    static float playerH; // Wysokoœæ gracza
    long int kills; // Wynik

    Map* map;

private:
    void UpdateAnimation(); // Aktualizacja animacji
    void renderHealthBar(double healthValue, SDL_Renderer* renderer); // Renderowanie paska zdrowia

    float x; // Pozycja x
    float y; // Pozycja y
    float velocityX; // Prêdkoœæ x
    float velocityY; // Prêdkoœæ y
    Camera* camera; // Kamera

    SDL_Texture* playerTexture; // Tekstura gracza
    int frameWidth; // Szerokoœæ klatki
    int frameHeight; // Wysokoœæ klatki
    int currentFrame; // Aktualna klatka
    int currentRow; // Aktualny wiersz
    int totalFrames; // Ca³kowita liczba klatek
    Uint32 lastFrameTime; // Ostatni czas klatki
    Uint32 frameDuration; // Czas trwania klatki

    double health; // Zdrowie
};