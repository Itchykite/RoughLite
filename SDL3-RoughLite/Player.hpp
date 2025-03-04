#pragma once

// Player odpowiada za gracza, jego zachowanie, wygl�d, kolizje

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

    void LoadTexture(SDL_Renderer* renderer, const char* pathFile); // Za�adowanie tekstury gracza
    void LoadAttackTexture(SDL_Renderer* renderer, const char* pathFile); // Za�adowanie tekstury ataku
    void Render(SDL_Renderer* renderer); // Renderowanie gracza
    void renderHealthBar(double healthValue, SDL_Renderer* renderer); // Renderowanie paska zdrowia
    void SetPosition(float x, float y); // Ustwienie pozycji gracza
    void Update(float deltaTime, GameStateRunning currentState); // Aktualizacja gracza
    void SetVelocity(float x, float y); // Ustwienie pr�dko�ci gracza
    float GetX() const; // Pobranie pozycji x 
    float GetY() const; // Pobranie pozycji y
    float GetVelocityX() const; // Pobranie pr�dko�ci x
    float GetVelocityY() const; // Pobranie pr�dko�ci y
	float GetHeight() const; // Pobranie wysoko�ci gracza
    void SetCurrentRow(int row); // Ustwienie aktualnego wiersza dla animacji
    void HandleCollision(); // Obs�uga kolizji
    void attack(SDL_Renderer* renderer, std::vector<std::unique_ptr<Enemy>>& enemies, float dirX, float dirY); // Atak
    void UpdateKillsTexture(SDL_Renderer* renderer);
    void Reset();

	SDL_FRect GetCollisionRect() const; // Pobranie prostok�ta kolizji
    SDL_Texture* playerTexture; // Tekstura gracza


    float x; // Pozycja x
    float y; // Pozycja y
    long int kills; // Wynik
    long int totalKills; // Ca�kowita liczba zabitych przeciwnik�w
    long int totalDeaths; // Ca�kowita liczba zabitych przeciwnik�w
    double health; // Zdrowie
    bool isGameOver;
    bool deathRegistered;
    Uint64 gameOverTime;

    SDL_Texture* texture; // Tekstura

    Map* map;

private:
    void UpdateAnimation(); // Aktualizacja animacji

    float velocityX; // Pr�dko�� x
    float velocityY; // Pr�dko�� y
    Camera* camera; // Kamera
    SDL_Texture* attackTexture; // Tekstura ataku
	TTF_Font* font; // Czcionka
    int frameWidth; // Szeroko�� klatki
    int frameHeight; // Wysoko�� klatki
    int currentFrame; // Aktualna klatka
    int currentRow; // Aktualny wiersz
    int totalFrames; // Ca�kowita liczba klatek
    Uint64 lastFrameTime; // Ostatni czas klatki
    Uint64 frameDuration; // Czas trwania klatki
    Uint64 attackFrameDuration; // Czas trwania ataku

    bool isAttacking; // Czy gracz atakuje
    int attackFrame; // Aktualna klatka ataku
    int attackRow; // Aktualny wiersz ataku
};