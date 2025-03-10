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
#include <fstream>

extern GameStateRunning gameState;
extern Uint64 lastTime;
extern void resetLastTime();

float yOffSet = 100.0f;
bool dropdownOpen = false;
bool vSyncOpen = false;
bool isLevelUp = true;
bool isFullscreen = false;
int selectedResolutionIndex = 3; 
int selectedFPSIndex = 1;

Button startButton(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 8, 200, 80, {255, 0, 0, 255}, []()
{
    SDL_Log("Start Game!");
    gameState = GameStateRunning::GAME;
    resetLastTime(); // Resetowanie lastTime poprzez wywo³anie funkcji
});

Button exitButton(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 24, 200, 80, { 255, 0, 0, 255 }, []()
{
    SDL_Log("Exit Game!");
    gameState = GameStateRunning::EXIT;
});

Button statsButton(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 24, 200, 80, { 255, 0, 0, 255 }, []()
{
    SDL_Log("Stats Game!");
	gameState = GameStateRunning::STATS;
});

Button settingsButton(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 24, 200, 80, { 255, 0, 0, 255 }, []()
{
    SDL_Log("Stats Game!");
    gameState = GameStateRunning::SETTINGS;
});

SDL_AppResult gameRunning(SDL_Renderer* renderer, Player* player, Map* map, Camera* camera, EnemyManager* enemyManager,
    const Uint64& startTime, Uint64& lastTime, SDL_Event& event, TTF_Font* font, void* appstate, GameStateRunning currentState)
{
    player->isGameStart = true;

    Uint64 currentTime = SDL_GetTicks();

    if (!player->isGameOver && currentState == GameStateRunning::GAME)
    {
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        enemyManager->Update(deltaTime, currentState);
        player->Update(deltaTime, currentState);

        camera->Update(player->GetX(), player->GetY());

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect viewport;
        viewport.x = 0;
        viewport.y = 0;
        viewport.w = WINDOW_WIDTH;
        viewport.h = WINDOW_HEIGHT;
        SDL_SetRenderViewport(renderer, &viewport);

        map->Render(renderer, camera->GetX(), camera->GetY());
        enemyManager->Render(renderer, currentState);
        map->RenderObjects(renderer, camera->GetX(), camera->GetY(), player);
        player->UpdateKillsTexture(renderer);

        SDL_FPoint playerPosition = { player->GetX(), player->GetY() };
        int playerX = static_cast<int>(playerPosition.x);
        int playerY = static_cast<int>(playerPosition.y);

        if (!map->IsPixelTransparent(playerX, playerY))
        {
            player->Update(-deltaTime, currentState);
        }
    }

    return SDL_APP_CONTINUE;
}

void GameOver(SDL_Renderer* renderer, TTF_Font* font, Player* player, Uint64& endTime, Uint64& startTime) // Funkcja wyœwietlaj¹ca napis Game Over
{
    player->isGameStart = false;

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

void gameMenu(SDL_Renderer* renderer, SDL_Event& event, TTF_Font* font, Player* player, Map* map, EnemyManager* enemyManager)
{
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_RenderClear(renderer);

    if (player->isGameStart)
    {
		gamePause(renderer, font);
    }

    float OffSet = 24.0f;

    // Renderowanie przycisku start
    SDL_Surface* startTextSurface = TTF_RenderText_Solid(font, "start", 0, { 255, 255, 255, 255 });
    if (startTextSurface)
    {
        int startTextWidth = startTextSurface->w;
        SDL_DestroySurface(startTextSurface);
        startButton.SetPosition(WINDOW_WIDTH / 2 - startButton.GetFRect().h - OffSet, WINDOW_HEIGHT / 2 - startButton.GetFRect().h - OffSet);
    }
    startButton.Render(renderer, font, "start", player, map, enemyManager);
    startButton.handleClick(event);

	// Renderowanie przycisku stats
    SDL_Surface* statsTextSurface = TTF_RenderText_Solid(font, "stats", 0, { 255, 255, 255, 255 });
    if (statsTextSurface)
    {
        int statsTextWidth = statsTextSurface->w;
        SDL_DestroySurface(statsTextSurface);
        statsButton.SetPosition(WINDOW_WIDTH / 2 - statsButton.GetFRect().h - OffSet, WINDOW_HEIGHT / 2);
    }
    statsButton.Render(renderer, font, "stats", player, map, enemyManager);
    statsButton.handleClick(event);

    // Renderowanei przycisku settings
    SDL_Surface* settingsTextSurface = TTF_RenderText_Solid(font, "settings", 0, { 255, 255, 255, 255 });
    if (settingsTextSurface)
    {
        SDL_DestroySurface(settingsTextSurface);
        settingsButton.SetPosition(WINDOW_WIDTH / 2 - settingsButton.GetFRect().h - OffSet, WINDOW_HEIGHT / 2 + settingsButton.GetFRect().h + OffSet);
    }
    settingsButton.Render(renderer, font, "settings", player, map, enemyManager);
    settingsButton.handleClick(event);

    // Renderowanie przycisku exit
    SDL_Surface* exitTextSurface = TTF_RenderText_Solid(font, "exit", 0, { 255, 255, 255, 255 });
    if (exitTextSurface)
    {
        int exitTextWidth = exitTextSurface->w;
        SDL_DestroySurface(exitTextSurface);
        exitButton.SetPosition(WINDOW_WIDTH / 2 - exitButton.GetFRect().h - OffSet, WINDOW_HEIGHT / 2 + exitButton.GetFRect().h + OffSet * 5 + 8.0f);
    }
    exitButton.Render(renderer, font, "exit", player, map, enemyManager);
    exitButton.handleClick(event);
    
    SDL_RenderPresent(renderer); // Renderowanie ekranu menu
}

void gamePause(SDL_Renderer* renderer, TTF_Font* font)
{
    // Renderowanie t³a pauzy
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128); // Pó³przezroczyste czarne t³o
    SDL_FRect pauseRect = { WINDOW_WIDTH / 2 - 100, 100, 200, 100 };
    SDL_RenderFillRect(renderer, &pauseRect);

    // Renderowanie napisu "PAUZA"
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "PAUZA", 0, white);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FRect textRect = { pauseRect.x + 50, pauseRect.y + 25, 100, 50 };
    SDL_RenderTexture(renderer, textTexture, NULL, &textRect);
    SDL_DestroySurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void gameStats(SDL_Renderer* renderer, TTF_Font* font, Player* player)
{
    SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_FRect statsRect = { WINDOW_WIDTH / 2 - 100, 100, 200, 100 };
    SDL_RenderFillRect(renderer, &statsRect);

    SDL_Color white = { 255, 255, 255, 255 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, "STATS", 0, white);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FRect textRect = { statsRect.x + 50, statsRect.y + 25, 100, 50 };
    SDL_RenderTexture(renderer, textTexture, NULL, &textRect);

    SDL_DestroySurface(textSurface);
    SDL_DestroyTexture(textTexture);

    statTemplate(renderer, font, player, "Total Kills", 0, player->totalKills);
    statTemplate(renderer, font, player, "Total Deaths", 2, player->totalDeaths);
	statTemplate(renderer, font, player, "Total Time", -2, player->totalTime / 100);
}

void statTemplate(SDL_Renderer* renderer, TTF_Font* font, Player* player, std::string statText, float offSet, long int value)
{
    SDL_FRect rect = { WINDOW_WIDTH / 2 - 100, 100, 200, 100 };

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, statText.c_str(), 0, white);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FRect textRect = { rect.x + 50, rect.y + 25, 100, 50 };

    float textWidth = textSurface->w;
    float textHeight = textSurface->h;

    std::string deathsText = statText + ": " + std::to_string(value);
    textSurface = TTF_RenderText_Solid(font, deathsText.c_str(), 0, white);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    textRect = { (WINDOW_WIDTH - textWidth) / 2, WINDOW_HEIGHT / 2 - textHeight * offSet, textWidth, textHeight };
    SDL_RenderTexture(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}

void statTemplate(SDL_Renderer* renderer, TTF_Font* font, Player* player, std::string statText, float offSet, Uint64 value)
{
    SDL_FRect rect = { WINDOW_WIDTH / 2 - 100, 100, 200, 100 };

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, statText.c_str(), 0, white);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FRect textRect = { rect.x + 50, rect.y + 25, 100, 50 };

    float textWidth = textSurface->w;
    float textHeight = textSurface->h;

    std::string deathsText = statText + ": " + std::to_string(value) + " s.";
    textSurface = TTF_RenderText_Solid(font, deathsText.c_str(), 0, white);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    textRect = { (WINDOW_WIDTH - textWidth) / 2, WINDOW_HEIGHT / 2 - textHeight * offSet, textWidth, textHeight };
    SDL_RenderTexture(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}

// Dodanie funkcji gameSettings
void gameSettings(SDL_Renderer* renderer, SDL_Event& event, TTF_Font* font, GameStateRunning& currentState, SDL_Window* window, Camera* camera, Player* player)
{
    bool inSettings = true;
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color gray = { 200, 200, 200, 255 };

    while (inSettings)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                currentState = GameStateRunning::EXIT;
                inSettings = false;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_ESCAPE)
                {
                    currentState = GameStateRunning::MENU;
                    inSettings = false;
                }
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                SDL_FRect fullscreenRect = { 700.0f, 100.0f, 200.0f, 30.0f };
                if (mouseX >= fullscreenRect.x && mouseX <= fullscreenRect.x + fullscreenRect.w &&
                    mouseY >= fullscreenRect.y && mouseY <= fullscreenRect.y + fullscreenRect.h)
                {
                    isFullscreen = !isFullscreen;
                    if (isFullscreen)
                    {
                        SDL_SetWindowFullscreen(window, SDL_GetWindowFullscreenMode);
                    }
                    else
                    {
                        SDL_SetWindowFullscreen(window, 0);
                    }

                    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_STRETCH);
                }

                // Handle mouse clicks
                // Check if the dropdown list is clicked
                SDL_FRect dropdownRect = { 100.0f, 100.0f, 200.0f, 30.0f };
                if (mouseX >= dropdownRect.x && mouseX <= dropdownRect.x + dropdownRect.w &&
                    mouseY >= dropdownRect.y && mouseY <= dropdownRect.y + dropdownRect.h)
                {
                    dropdownOpen = !dropdownOpen;
                }

                // If the list is open, check if an option is selected
                if (dropdownOpen)
                {
                    for (size_t i = 0; i < availableResolutions.size(); ++i)
                    {
                        SDL_FRect optionRect = { 100.0f, 130.0f + 30.0f * i, 200.0f, 30.0f };
                        if (mouseX >= optionRect.x && mouseX <= optionRect.x + optionRect.w &&
                            mouseY >= optionRect.y && mouseY <= optionRect.y + optionRect.h)
                        {
                            selectedResolutionIndex = static_cast<int>(i);
                            currentResolution = availableResolutions[i];
                            dropdownOpen = false;

                            // Change window size
                            SDL_SetWindowSize(window, currentResolution.width, currentResolution.height);

                            // Update the renderer's logical presentation
                            SDL_SetRenderLogicalPresentation(renderer, currentResolution.width, currentResolution.height, SDL_LOGICAL_PRESENTATION_STRETCH);

                            // Update window size variables
                            WINDOW_WIDTH = currentResolution.width;
                            WINDOW_HEIGHT = currentResolution.height;

                            // Update the camera's viewport size
                            camera->SetViewportSize(WINDOW_WIDTH, WINDOW_HEIGHT);

                            // Update the renderer's viewport
                            SDL_SetRenderViewport(renderer, NULL);

                            // Update the camera's thresholds
                            camera->thresholdX = camera->width / 2.0f;
                            camera->thresholdY = camera->height / 2.0f;

                            // Update the camera's position
                            camera->Update(player->GetX(), player->GetY());
                        }
                    }
                }

                SDL_FRect vSyncRect = { 400.0f, 100.0f, 200.0f, 30.0f };
                if (mouseX >= vSyncRect.x && mouseX <= vSyncRect.x + vSyncRect.w &&
                    mouseY >= vSyncRect.y && mouseY <= vSyncRect.y + vSyncRect.h)
                {
                    vSyncOpen = !vSyncOpen;
                }

                if (vSyncOpen)
                {
                    for (size_t i = 0; i < availableFPS.size(); ++i)
                    {
                        SDL_FRect optionRect = { 400.0f, 130.0f + 30.0f * i, 200.0f, 30.0f };
                        if (mouseX >= optionRect.x && mouseX <= optionRect.x + optionRect.w &&
                            mouseY >= optionRect.y && mouseY <= optionRect.y + optionRect.h)
                        {
                            selectedFPSIndex = static_cast<int>(i);
                            fps_t = availableFPS[i];
                            vSyncOpen = false;
                        }
                    }
                }
            }
            else if (event.type >= SDL_EVENT_WINDOW_FIRST && event.type <= SDL_EVENT_WINDOW_LAST)
            {
                // Handle window events
                if (event.type == SDL_EVENT_WINDOW_RESIZED)
                {
                    WINDOW_WIDTH = event.window.data1;
                    WINDOW_HEIGHT = event.window.data2;

                    // Update the camera's viewport size
                    camera->SetViewportSize(WINDOW_WIDTH, WINDOW_HEIGHT);

                    // Update the camera's thresholds
                    camera->thresholdX = camera->width / 2.0f;
                    camera->thresholdY = camera->height / 2.0f;

                    // Update the renderer's viewport
                    SDL_SetRenderViewport(renderer, NULL);

                    // Update the camera's position
                    camera->Update(player->GetX(), player->GetY());
                }
                else if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED)
                {
                    // Handle changes in the window's back buffer size
                    SDL_SetRenderViewport(renderer, NULL);
                }
            }
            // Add handling for other relevant events
        }

        // Render the settings background
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);

        // Render the dropdown list
        SDL_FRect dropdownRect = { 100, 100, 200, 40 };
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &dropdownRect);

        // Display the selected resolution
        std::string selectedResolutionText = std::to_string(currentResolution.width) + " x " + std::to_string(currentResolution.height);
        SDL_Surface* selectedSurface = TTF_RenderText_Solid(font, selectedResolutionText.c_str(), 0, white);
        SDL_Texture* selectedTexture = SDL_CreateTextureFromSurface(renderer, selectedSurface);
        SDL_FRect textRect = { dropdownRect.x + 10, dropdownRect.y + 5, static_cast<float>(selectedSurface->w), static_cast<float>(selectedSurface->h) };
        SDL_RenderTexture(renderer, selectedTexture, NULL, &textRect);
        SDL_DestroySurface(selectedSurface);
        SDL_DestroyTexture(selectedTexture);

        SDL_FRect vSyncRect = { 400, 100, 200, 40 };
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &vSyncRect);

        // Display the selected FPS
        std::string selectedFPSText = std::to_string(static_cast<int>(fps_t));
        SDL_Surface* FPSselectedSurface = TTF_RenderText_Solid(font, selectedFPSText.c_str(), 0, white);
        SDL_Texture* FPSselectedTexture = SDL_CreateTextureFromSurface(renderer, FPSselectedSurface);
        SDL_FRect FPSTextRect = { vSyncRect.x + 10, vSyncRect.y + 5, static_cast<float>(FPSselectedSurface->w), static_cast<float>(FPSselectedSurface->h) };
        SDL_RenderTexture(renderer, FPSselectedTexture, NULL, &FPSTextRect);
        SDL_DestroySurface(FPSselectedSurface);
        SDL_DestroyTexture(FPSselectedTexture);

        // Render the fullscreen button
        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
        SDL_FRect fullscreenButtonRect = { 700, 100, 200, 40 };
        SDL_RenderFillRect(renderer, &fullscreenButtonRect);

        std::string fullscreenText = isFullscreen ? "Full Screen OFF" : "Full Screen ON";
        SDL_Surface* fullscreenSurface = TTF_RenderText_Solid(font, fullscreenText.c_str(), 0, white);
        SDL_Texture* fullscreenTexture = SDL_CreateTextureFromSurface(renderer, fullscreenSurface);
        SDL_FRect fullscreenTextRect = { fullscreenButtonRect.x + 10, fullscreenButtonRect.y + 5, static_cast<float>(fullscreenSurface->w), static_cast<float>(fullscreenSurface->h) };
        SDL_RenderTexture(renderer, fullscreenTexture, NULL, &fullscreenTextRect);
        SDL_DestroySurface(fullscreenSurface);
        SDL_DestroyTexture(fullscreenTexture);

        // If the list is open, display the options
        if (dropdownOpen)
        {
            for (size_t i = 0; i < availableResolutions.size(); ++i)
            {
                SDL_FRect optionRect = { 100, 130 + 30 * i, 200, 30 };
                SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
                SDL_RenderFillRect(renderer, &optionRect);

                std::string optionText = std::to_string(availableResolutions[i].width) + " x " + std::to_string(availableResolutions[i].height);
                SDL_Surface* optionSurface = TTF_RenderText_Solid(font, optionText.c_str(), 0, white);
                SDL_Texture* optionTexture = SDL_CreateTextureFromSurface(renderer, optionSurface);
                SDL_FRect optionTextRect = { optionRect.x + 10, optionRect.y + 5, static_cast<float>(optionSurface->w), static_cast<float>(optionSurface->h) };
                SDL_RenderTexture(renderer, optionTexture, NULL, &optionTextRect);
                SDL_DestroySurface(optionSurface);
                SDL_DestroyTexture(optionTexture);
            }
        }

        if (vSyncOpen)
        {
            for (size_t i = 0; i < availableFPS.size(); ++i)
            {
                SDL_FRect optionRect = { 400, 130 + 30 * i, 200, 30 };
                SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
                SDL_RenderFillRect(renderer, &optionRect);

                std::string optionText = std::to_string(static_cast<int>(availableFPS[i]));
                SDL_Surface* optionSurface = TTF_RenderText_Solid(font, optionText.c_str(), 0, white);
                SDL_Texture* optionTexture = SDL_CreateTextureFromSurface(renderer, optionSurface);
                SDL_FRect optionTextRect = { optionRect.x + 10, optionRect.y + 5, static_cast<float>(optionSurface->w), static_cast<float>(optionSurface->h) };
                SDL_RenderTexture(renderer, optionTexture, NULL, &optionTextRect);
                SDL_DestroySurface(optionSurface);
                SDL_DestroyTexture(optionTexture);
            }
        }

        SDL_RenderPresent(renderer);
    }
}

void updateButtonPositions()
{
    float OffSet = 24.0f;

    startButton.SetPosition(WINDOW_WIDTH / 2 - startButton.GetFRect().w / 2, WINDOW_HEIGHT / 2 - startButton.GetFRect().h - OffSet);
    statsButton.SetPosition(WINDOW_WIDTH / 2 - statsButton.GetFRect().w / 2, WINDOW_HEIGHT / 2);
    settingsButton.SetPosition(WINDOW_WIDTH / 2 - settingsButton.GetFRect().w / 2, WINDOW_HEIGHT / 2 + settingsButton.GetFRect().h + OffSet);
    exitButton.SetPosition(WINDOW_WIDTH / 2 - exitButton.GetFRect().w / 2, WINDOW_HEIGHT / 2 + exitButton.GetFRect().h + OffSet * 5 + 8.0f);
}

void levelUp(SDL_Renderer* renderer, TTF_Font* font, Player* player, SDL_Event& event, GameStateRunning& currentState, EnemyManager* enemyManager)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_FRect levelUpRect = { WINDOW_WIDTH / 2 - 150, 100, 300, 100 };
    SDL_RenderFillRect(renderer, &levelUpRect);

    // Renderowanie napisu "PAUZA"
	SDL_Color gold = { 255, 215, 0, 255 };
    SDL_Color black = { 0, 0, 0, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "LEVEL UP!", 0, gold);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FRect textRect = { levelUpRect.x, levelUpRect.y, 300, 50 };
    SDL_RenderTexture(renderer, textTexture, NULL, &textRect);
    SDL_DestroySurface(textSurface);
    SDL_DestroyTexture(textTexture);

    // Wiêcej zdrowia
    SDL_FRect healthBoostRect = { WINDOW_WIDTH / 4 - WINDOW_WIDTH / 6, WINDOW_HEIGHT / 2, 180, 180 };
    SDL_RenderFillRect(renderer, &healthBoostRect);

	// Wiêkszy zasiêg
    SDL_FRect rangeBoostRect = { WINDOW_WIDTH / 4 * 2 - WINDOW_WIDTH / 6, WINDOW_HEIGHT / 2, 180, 180 };
    SDL_RenderFillRect(renderer, &rangeBoostRect);

	// Wiêksza prêdkoœæ
    SDL_FRect speedBoostRect = { WINDOW_WIDTH / 4 * 3 - WINDOW_WIDTH / 6, WINDOW_HEIGHT / 2, 180, 180 };
    SDL_RenderFillRect(renderer, &speedBoostRect);

	// Wiêksze obra¿enia
    SDL_FRect damageBoostRect = { WINDOW_WIDTH / 4  * 4 - WINDOW_WIDTH / 6, WINDOW_HEIGHT / 2, 180, 180 };
    SDL_RenderFillRect(renderer, &damageBoostRect);

	// Renderowanie upgrade zdrowia
	SDL_Surface* healthTextSurface = TTF_RenderText_Solid(font, "Health Boost", 0, black);
	SDL_Texture* healthTexture = SDL_CreateTextureFromSurface(renderer, healthTextSurface);
	SDL_FRect healthTextRect = { healthBoostRect.x, healthBoostRect.y + 180, 180, 50 };

	SDL_RenderTexture(renderer, healthTexture, NULL, &healthTextRect);
	SDL_DestroySurface(healthTextSurface);
	SDL_DestroyTexture(healthTexture);

	// Renderowanie upgrade zasiêgu
	SDL_Surface* rangeTextSurface = TTF_RenderText_Solid(font, "Range Boost", 0, black);
	SDL_Texture* rangeTexture = SDL_CreateTextureFromSurface(renderer, rangeTextSurface);
	SDL_FRect rangeTextRect = { rangeBoostRect.x, rangeBoostRect.y + 180, 180, 50 };

	SDL_RenderTexture(renderer, rangeTexture, NULL, &rangeTextRect);
	SDL_DestroySurface(rangeTextSurface);
	SDL_DestroyTexture(rangeTexture);

	// Renderowanie upgrade prêdkoœci
	SDL_Surface* speedTextSurface = TTF_RenderText_Solid(font, "Speed Boost", 0, black);
	SDL_Texture* speedTexture = SDL_CreateTextureFromSurface(renderer, speedTextSurface);
	SDL_FRect speedTextRect = { speedBoostRect.x, speedBoostRect.y + 180, 180, 50 };

	SDL_RenderTexture(renderer, speedTexture, NULL, &speedTextRect);
	SDL_DestroySurface(speedTextSurface);
	SDL_DestroyTexture(speedTexture);

	// Renderowanie upgrade obra¿eñ
	SDL_Surface* damageTextSurface = TTF_RenderText_Solid(font, "Damage Boost", 0, black);
	SDL_Texture* damageTexture = SDL_CreateTextureFromSurface(renderer, damageTextSurface);
	SDL_FRect damageTextRect = { damageBoostRect.x, damageBoostRect.y + 180, 180, 50 };

	SDL_RenderTexture(renderer, damageTexture, NULL, &damageTextRect);
	SDL_DestroySurface(damageTextSurface);
	SDL_DestroyTexture(damageTexture);
	SDL_RenderPresent(renderer);

    int mouseX = event.button.x;
    int mouseY = event.button.y;
        
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (mouseX >= healthBoostRect.x && mouseX <= healthBoostRect.x + healthBoostRect.w &&
            mouseY >= healthBoostRect.y && mouseY <= healthBoostRect.y + healthBoostRect.h)
        {
            player->maxHealth *= 1.5f;
			SDL_Log("Health Boost! %f", player->health);
            isLevelUp = false;
        }

        if (mouseX >= rangeBoostRect.x && mouseX <= rangeBoostRect.x + rangeBoostRect.w &&
            mouseY >= rangeBoostRect.y && mouseY <= rangeBoostRect.y + rangeBoostRect.h)
        {
            player->attackRange *= 1.1f;
			SDL_Log("Range Boost!");
            isLevelUp = false;
        }

        if (mouseX >= speedBoostRect.x && mouseX <= speedBoostRect.x + speedBoostRect.w &&
            mouseY >= speedBoostRect.y && mouseY <= speedBoostRect.y + speedBoostRect.h)
        {
            //player->speed += 50;
			SDL_Log("Speed Boost!");
            isLevelUp = false;
        }

        if (mouseX >= damageBoostRect.x && mouseX <= damageBoostRect.x + damageBoostRect.w &&
            mouseY >= damageBoostRect.y && mouseY <= damageBoostRect.y + damageBoostRect.h)
        {
            //player->damage += 50;
			SDL_Log("Damage Boost!");
            isLevelUp = false;
        }
    }

    if (!isLevelUp)
    {
        player->health = player->maxHealth;
        enemyManager->lastCollisionTime = 0;
        isLevelUp = true;
        currentState = GameStateRunning::GAME;
    }
}

void savePlayerStats(Player* player)
{
	std::ofstream saveFile("playerstats.dat", std::ios::binary);
	if (saveFile.is_open())
	{
        saveFile.write(reinterpret_cast<char*>(&player->totalKills), sizeof(player->totalKills));
        saveFile.write(reinterpret_cast<char*>(&player->totalDeaths), sizeof(player->totalDeaths));
		saveFile.close();
	}
}

void loadPlayerStats(Player* player)
{
	std::ifstream loadFile("playerstats.dat", std::ios::binary);
	if (loadFile.is_open())
	{
		loadFile.read(reinterpret_cast<char*>(&player->totalKills), sizeof(player->totalKills));
        loadFile.read(reinterpret_cast<char*>(&player->totalDeaths), sizeof(player->totalDeaths));
		loadFile.close();
	}
}

void saveGameState(Player* player, Map* map, EnemyManager* enemyManager)
{
    std::ofstream saveFile("savegame.dat", std::ios::binary);

    float playerX = player->GetX();
    float playerY = player->GetY();

    if (saveFile.is_open())
    {
        // Zapisz stan gracza
        saveFile.write(reinterpret_cast<char*>(&playerX), sizeof(playerX));
        saveFile.write(reinterpret_cast<char*>(&playerY), sizeof(playerY));
        saveFile.write(reinterpret_cast<char*>(&player->health), sizeof(player->health));
        saveFile.write(reinterpret_cast<char*>(&player->kills), sizeof(player->kills));

        // Zapisz stan przeciwników
        int numEnemies = enemyManager->enemies.size();
        saveFile.write(reinterpret_cast<char*>(&numEnemies), sizeof(numEnemies));

        for (const auto& enemy : enemyManager->enemies)
        {
            float enemyX = enemy->GetX();
            float enemyY = enemy->GetY();
            saveFile.write(reinterpret_cast<const char*>(&enemyX), sizeof(enemyX));
            saveFile.write(reinterpret_cast<const char*>(&enemyY), sizeof(enemyY));
        }

        saveFile.close();
    }
}

void loadGameState(Player* player, Map* map, EnemyManager* enemyManager, SDL_Renderer* renderer)
{
    std::ifstream loadFile("savegame.dat", std::ios::binary);

    if (loadFile.is_open())
    {
        // Wczytaj stan gracza
        loadFile.read(reinterpret_cast<char*>(&player->x), sizeof(player->x));
        loadFile.read(reinterpret_cast<char*>(&player->y), sizeof(player->y));
        loadFile.read(reinterpret_cast<char*>(&player->health), sizeof(player->health));
        loadFile.read(reinterpret_cast<char*>(&player->kills), sizeof(player->kills));

        // Wczytaj stan przeciwników
        int numEnemies;
        loadFile.read(reinterpret_cast<char*>(&numEnemies), sizeof(numEnemies));

        enemyManager->enemies.clear();

        for (int i = 0; i < numEnemies; ++i) 
        {
            float enemyX, enemyY;
            loadFile.read(reinterpret_cast<char*>(&enemyX), sizeof(enemyX));
            loadFile.read(reinterpret_cast<char*>(&enemyY), sizeof(enemyY));
            auto enemy = std::make_unique<Enemy>(player, map, enemyManager->camera, enemyManager->renderer);
            enemy->SetPosition(enemyX, enemyY);
            enemyManager->enemies.push_back(std::move(enemy));
        }

        loadFile.close();
    }
}