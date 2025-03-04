#pragma once

// Player odpowiada za gracza, jego zachowanie, wygl¹d, kolizje

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Map.hpp"
#include "Camera.hpp"
#include <memory>
#include <vector>
#include "GameState.hpp"

class Enemy;

class Player
{
public:
    Player(Map* map, Camera* camera, SDL_Renderer* renderer); // Konstruktor gracza, mapa, kamera, renderer
    ~Player(); // Destruktor

    void LoadTexture(SDL_Renderer* renderer, const char* pathFile); // Za³adowanie tekstury gracza
    void LoadAttackTexture(SDL_Renderer* renderer, const char* pathFile); // Za³adowanie tekstury ataku
    void Render(SDL_Renderer* renderer); // Renderowanie gracza
    void renderHealthBar(double healthValue, SDL_Renderer* renderer); // Renderowanie paska zdrowia
    void SetPosition(float x, float y); // Ustwienie pozycji gracza
    void Update(float deltaTime, GameStateRunning currentState); // Aktualizacja gracza
    void SetVelocity(float x, float y); // Ustwienie prêdkoœci gracza
    float GetX() const; // Pobranie pozycji x 
    float GetY() const; // Pobranie pozycji y
    float GetVelocityX() const; // Pobranie prêdkoœci x
    float GetVelocityY() const; // Pobranie prêdkoœci y
	float GetHeight() const; // Pobranie wysokoœci gracza
    void SetCurrentRow(int row); // Ustwienie aktualnego wiersza dla animacji
    void HandleCollision(); // Obs³uga kolizji
    void attack(SDL_Renderer* renderer, std::vector<std::unique_ptr<Enemy>>& enemies, float dirX, float dirY); // Atak
    void UpdateKillsTexture(SDL_Renderer* renderer);
    void Reset();

	SDL_FRect GetCollisionRect() const; // Pobranie prostok¹ta kolizji
    SDL_Texture* playerTexture; // Tekstura gracza


    float x; // Pozycja x
    float y; // Pozycja y
    long int kills; // Wynik
    long int totalKills; // Ca³kowita liczba zabitych przeciwników
    long int totalDeaths; // Ca³kowita liczba zabitych przeciwników
    double health; // Zdrowie
    bool isGameOver;
    bool deathRegistered;
    Uint64 gameOverTime;

    SDL_Texture* texture; // Tekstura

    Map* map;

private:
    void UpdateAnimation(); // Aktualizacja animacji

    float velocityX; // Prêdkoœæ x
    float velocityY; // Prêdkoœæ y
    Camera* camera; // Kamera
    SDL_Texture* attackTexture; // Tekstura ataku
	TTF_Font* font; // Czcionka
    int frameWidth; // Szerokoœæ klatki
    int frameHeight; // Wysokoœæ klatki
    int currentFrame; // Aktualna klatka
    int currentRow; // Aktualny wiersz
    int totalFrames; // Ca³kowita liczba klatek
    Uint64 lastFrameTime; // Ostatni czas klatki
    Uint64 frameDuration; // Czas trwania klatki
    Uint64 attackFrameDuration; // Czas trwania ataku

    bool isAttacking; // Czy gracz atakuje
    int attackFrame; // Aktualna klatka ataku
    int attackRow; // Aktualny wiersz ataku
};