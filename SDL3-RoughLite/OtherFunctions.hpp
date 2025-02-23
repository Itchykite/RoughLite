#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Player.hpp"

// void GameTime(); // Funkcja czasu gry
void RenderGameOverScreen(SDL_Renderer* renderer, Player* player, Uint32 startTime); // Funkcja renderowania ekranu koñca gry