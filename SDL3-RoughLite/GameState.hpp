#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class Player;
class Map;
class Camera;
class EnemyManager;

enum class GameStateRunning
{
    MENU,
    GAME,
    GAMEOVER,
    EXIT
};

SDL_AppResult gameRunning(SDL_Renderer* renderer, Player* player, Map* map, Camera* camera, EnemyManager* enemyManager,
    const Uint64& startTime, Uint64& lastTime, void* appstate);

void GameOver(SDL_Renderer* renderer, TTF_Font* font, Player* player, Uint64& endTime, Uint64& startTime);

void gameMenu(SDL_Renderer* renderer, SDL_Event& event, TTF_Font* font);