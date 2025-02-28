#include "GameState.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "Settings.hpp" // Postawowe ustwienia
#include "Player.hpp" // Gracz
#include "PlayerEventHandling.hpp" // Przechwytywanie przycisk�w
#include "Map.hpp" // Mapa
#include "Camera.hpp" // Kamera
#include "Enemy.hpp" // Przeciwnik
#include "EnemyManager.hpp" // Mened�er przeciwnik�w (Dodawnie konkretnej ilo�ci)
#include "RangeRover.hpp" // Kr�c�cy si� ziut
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
    if (player->isGameOver) // Je�li gra jest zako�czona
    {
        const Uint64 endTime = SDL_GetTicks(); // Pobranie czasu zako�czenia gry
        RenderGameOverScreen(renderer, player, endTime, startTime); // Renderowanie ekranu "Game Over"
        return SDL_APP_CONTINUE;
    }

    Uint64 currentTime = SDL_GetTicks(); // Pobiera aktualny czas
    float deltaTime = (currentTime - lastTime) / 1000.0f; // Czas co sekunde
    lastTime = currentTime; // Pobiera ostatnio zapisany czas, z przed sekundy

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Koloruje ekran na czarno
    SDL_RenderClear(renderer); // i go renderuje 

    map->Render(renderer, camera->GetX(), camera->GetY()); // Renderowanie

    enemyManager->Update(deltaTime); // Aktualizacja przeciwnika co sekunde
    enemyManager->Render(renderer); // Renerowanie przeciwnik�w

    player->Update(deltaTime); // Aktulizacja gracza co sekunde
    map->RenderObjects(renderer, camera->GetX(), camera->GetY(), player); // Renderowanie obiekt�w
    player->UpdateKillsTexture(renderer); // Aktualizacja wyniku

    SDL_FPoint playerPosition = { player->GetX(), player->GetY() }; // Pobieranie aktualnej pozycji gracza jako punkt
    int playerX = static_cast<int>(playerPosition.x); // przechowanie aktualnego x dla gracza, int
    int playerY = static_cast<int>(playerPosition.y); // przechowanie aktualnego y dla gracza, int

    if (!map->IsPixelTransparent(playerX, playerY))
    {
        player->Update(-deltaTime);
    }

    if (player->health <= 0) // Je�li zdrowie gracza jest mniejsze lub r�wne 0
    {
        player->isGameOver = true; // Gra si� ko�czy
        return SDL_APP_CONTINUE;
    }
}

void GameOver(SDL_Renderer* renderer, TTF_Font* font, Player* player) // Funkcja wy�wietlaj�ca napis Game Over
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
}

void gameMenu(SDL_Renderer* renderer, SDL_Event& event, TTF_Font* font)
{
	startButton.Render(renderer, font, "start"); // Renderowanie przycisku start
	startButton.handleClick(event); // Obs�uga klikni�cia przycisku start

	exitButton.Render(renderer, font, "exit"); // Renderowanie przycisku exit
	exitButton.handleClick(event); // Obs�uga klikni�cia przycisku exit
}