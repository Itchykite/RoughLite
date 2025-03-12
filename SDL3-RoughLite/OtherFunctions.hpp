#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Player.hpp"

// void GameTime(); // Funkcja czasu gry
void RenderGameOverScreen(SDL_Renderer* renderer, Player* player, const Uint64& gameOverTime, const Uint64& startTime); // Funkcja renderowania ekranu koñca gry
<<<<<<< HEAD
void showFPS(SDL_Renderer* renderer, Uint64& lastFrameTime, Uint64& frameCount, double& fps, TTF_Font* font);
void saveGameTime(Player*& player, const Uint64& startTime);
=======
void saveGameTime(Player*& player, const Uint64& startTime); // Funkcja zapisu czasu gry
>>>>>>> 6e0ba8fd25343989c66d0b91996e91101feb47fd
void loadGameTime(Player*& player);