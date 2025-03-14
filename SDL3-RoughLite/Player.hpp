#pragma once

// Player odpowiada za gracza, jego zachowanie, wygląd, kolizje

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

    void LoadTexture(SDL_Renderer* renderer, const char* pathFile); // Załadowanie tekstury gracza
    void LoadAttackTexture(SDL_Renderer* renderer, const char* pathFile); // Załadowanie tekstury ataku
    void Render(SDL_Renderer* renderer); // Renderowanie gracza
    void renderHealthBar(double healthValue, SDL_Renderer* renderer, TTF_Font* font); // Renderowanie paska zdrowia
	void renderExpBar(double expValue, SDL_Renderer* renderer, TTF_Font* font); // Renderowanie paska expa
    void SetPosition(float x, float y); // Ustwienie pozycji gracza
    void Update(float deltaTime, GameStateRunning currentState); // Aktualizacja gracza
    void SetVelocity(float x, float y); // Ustwienie prędkości gracza
    float GetX() const; // Pobranie pozycji x 
    float GetY() const; // Pobranie pozycji y
    float GetVelocityX() const; // Pobranie prędkości x
    float GetVelocityY() const; // Pobranie prędkości y
	float GetHeight() const; // Pobranie wysokości gracza
    void SetCurrentRow(int row); // Ustwienie aktualnego wiersza dla animacji
    void HandleCollision(); // Obsługa kolizji
    void attack(SDL_Renderer* renderer, std::vector<std::unique_ptr<Enemy>>& enemies, float dirX, float dirY); // Atak
    void UpdateKillsTexture(SDL_Renderer* renderer);
    void Reset();

	SDL_FRect GetCollisionRect() const; // Pobranie prostokąta kolizji
    SDL_Texture* playerTexture; // Tekstura gracza


    float attackRange;
    float x; // Pozycja x
    float y; // Pozycja y
    long int kills; // Wynik
    long int totalKills; // Całkowita liczba zabitych przeciwników
    long int totalDeaths; // Całkowita liczba zabitych przeciwników
	Uint64 totalTime; // Całkowity czas gry
	double maxHealth; // Maksymalne zdrowie
    double health; // Zdrowie
	double exp; // Doświadczenie
	double maxExp; // Maksymalne doświadczenie
    bool isGameOver;
    bool isGameStart = false;
    bool deathRegistered;
    bool wasMoving;
    Uint64 gameOverTime;

    float velocityX; // Prędkość x
    float velocityY; // Prędkość y
    float velocityBoost;
	float speed; // Prędkość
	float damage; // Obrażenia

    SDL_Texture* texture; // Tekstura

    Map* map;

private:
    void UpdateAnimation(); // Aktualizacja animacji
	void UpdateAttackAnimation(); // Aktualizacja animacji ataku

    Camera* camera; // Kamera
    SDL_Texture* attackTexture; // Tekstura ataku
	TTF_Font* font; // Czcionka
    int frameWidth; // Szerokość klatki
    int frameHeight; // Wysokość klatki
    int currentFrame; // Aktualna klatka
    int currentRow; // Aktualny wiersz
    int totalFrames; // Całkowita liczba klatek
    Uint64 lastFrameTime; // Ostatni czas klatki
    Uint64 frameDuration; // Czas trwania klatki
    Uint64 attackFrameDuration; // Czas trwania ataku

    bool isAttacking; // Czy gracz atakuje
    int attackFrame; // Aktualna klatka ataku
    int attackRow; // Aktualny wiersz ataku

};