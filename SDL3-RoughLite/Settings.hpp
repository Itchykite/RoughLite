#pragma once

#include <vector>

struct Resolution
{
    int width;
    int height;
};

extern std::vector<Resolution> availableResolutions;
extern Resolution currentResolution;

extern float WINDOW_WIDTH;
extern float WINDOW_HEIGHT;

// dla mapy dla rodziny 16:9

static const float mapWidth = 4096;
static const float mapHeight = 4096;

static const float playerW = 128.0f; // Ustawienie szerokoœci gracza
static const float playerH = 128.0f; // Ustawienie wysokoœci gracza

static const float enemyW = 128.0f; // Ustawienie szerokoœci przeciwnika
static const float enemyH = 128.0f; // Ustawienie wysokoœci przeciwnika

// Delay = 1000 / fps

// 30 - 33.33f
// 60 - 16.67f
// 120 - 8.33f
// 144 - 6.94f
// 240 - 4.17f

static float fps = 6.94f;