#include "GameState.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "Settings.hpp" // Postawowe ustwienia
#include "Player.hpp" // Gracz
#include "PlayerEventHandling.hpp" // Przechwytywanie przycisków
#include "Map.hpp" // Mapa
#include "Camera.hpp" // Kamera
#include "Enemy.hpp" // Przeciwnik
#include "EnemyManager.hpp" // Mened¿er przeciwników (Dodawnie konkretnej iloœci)
#include "RangeRover.hpp" // Krêc¹cy siê ziut
#include "OtherFunctions.hpp" // Inne funkcje
#include "Buttons.hpp"

#include <vector>
#include <memory>
#include <iostream>

extern GameStateRunning gameState;

Button startButton(WINDOW_WIDTH / 2 - WINDOW_WIDTH / 32, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 8, 200, 80, { 255, 0, 0, 255 }, []()
{
    SDL_Log("Start Game!");
    gameState = GameStateRunning::GAME;
});

Button exitButton(WINDOW_WIDTH / 2 - WINDOW_WIDTH / 32, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 24, 200, 80, { 255, 0, 0, 255 }, []()
{
    SDL_Log("Exit Game!");
    gameState = GameStateRunning::EXIT;
});

SDL_AppResult gameRunning(SDL_Renderer* renderer, Player* player, Map* map, Camera* camera, EnemyManager* enemyManager,
    const Uint64& startTime, Uint64& lastTime, void* appstate)
{
    if (gameState == GameStateRunning::MENU)
    {
        return SDL_APP_CONTINUE;
    }

    Uint64 currentTime = SDL_GetTicks(); // Pobiera aktualny czas
    float deltaTime = (currentTime - lastTime) / 1000.0f; // Czas co sekunde

    if (!player->isGameOver || gameState == GameStateRunning::MENU)
    {
        lastTime = currentTime; // Aktualizuj czas tylko jeœli gra nie jest skoñczona i nie jest w stanie pauzy

        enemyManager->Update(deltaTime); // Aktualizacja przeciwnika co sekunde
        player->Update(deltaTime); // Aktualizacja gracza co sekunde
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Koloruje ekran na czarno
    SDL_RenderClear(renderer); // i go renderuje 

    map->Render(renderer, camera->GetX(), camera->GetY()); // Renderowanie
    enemyManager->Render(renderer); // Renderowanie przeciwników
    map->RenderObjects(renderer, camera->GetX(), camera->GetY(), player); // Renderowanie obiektów
    player->UpdateKillsTexture(renderer); // Aktualizacja wyniku

    SDL_FPoint playerPosition = { player->GetX(), player->GetY() }; // Pobieranie aktualnej pozycji gracza jako punkt
    int playerX = static_cast<int>(playerPosition.x); // przechowanie aktualnego x dla gracza, int
    int playerY = static_cast<int>(playerPosition.y); // przechowanie aktualnego y dla gracza, int

    if (!map->IsPixelTransparent(playerX, playerY))
    {
        player->Update(-deltaTime);
    }

    if (player->health <= 0) // Jeœli zdrowie gracza jest mniejsze lub równe 0
    {
        player->isGameOver = true; // Gra siê koñczy
        gameState = GameStateRunning::GAMEOVER; // Gra siê koñczy
    }

    return SDL_APP_CONTINUE;
}

void GameOver(SDL_Renderer* renderer, TTF_Font* font, Player* player, Uint64& endTime, Uint64& startTime) // Funkcja wyœwietlaj¹ca napis Game Over
{
    SDL_Color textColor = { 255, 255, 255, 255 };

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Game Over", 0, textColor);
    if (!textSurface)
    {
        SDL_Log("Unable to render text surface: %s", SDL_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!texture)
    {
        SDL_Log("Unable to create texture from rendered text: %s", SDL_GetError());
    }

    SDL_FRect dst;
    dst.w = textSurface->w;
    dst.h = textSurface->h;
    dst.x = (WINDOW_WIDTH - dst.w) / 2;
    dst.y = (WINDOW_HEIGHT - dst.h) / 2;
    SDL_RenderTexture(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(textSurface);

    player->isGameOver = true;

    RenderGameOverScreen(renderer, player, endTime, startTime); // Renderowanie ekranu "Game Over"
}

void gameMenu(SDL_Renderer* renderer, SDL_Event& event, TTF_Font* font)
{
	startButton.Render(renderer, font, "start"); // Renderowanie przycisku start
	startButton.handleClick(event); // Obs³uga klikniêcia przycisku start

	exitButton.Render(renderer, font, "exit"); // Renderowanie przycisku exit
	exitButton.handleClick(event); // Obs³uga klikniêcia przycisku exit

    SDL_RenderPresent(renderer); // Renderowanie ekranu menu
}