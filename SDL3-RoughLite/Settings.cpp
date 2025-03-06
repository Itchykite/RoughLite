#include <tuple>
#include "Settings.hpp"
#include "SDL3/SDL.h"

std::vector<Resolution> availableResolutions =
{
	{ 640, 480 },
	{ 800, 600 },
	{ 1024, 768 },
	{ 1280, 720 },
	{ 1280, 768 },
	{ 1280, 800 },
	{ 1280, 1024 },
	{ 1360, 768 },
	{ 1366, 768 },
	{ 1440, 900 },
	{ 1600, 900 },
	{ 1680, 1050 },
	{ 1920, 1080 },
	{ 1920, 1200 },
	{ 2560, 1440 },
	{ 3840, 2160 }
};

Resolution currentResolution = availableResolutions[12]; // Domy�lna rozdzielczo��

float WINDOW_WIDTH = currentResolution.width;
float WINDOW_HEIGHT = currentResolution.height;

std::pair<float, float> GetWindowSize() 
{
    SDL_DisplayID display = SDL_GetPrimaryDisplay();

    int num_modes = 0;
    SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(display, &num_modes);
    if (modes == nullptr || num_modes == 0) 
	{
        SDL_Log("Nie uda�o si� pobra� tryb�w wy�wietlania lub brak dost�pnych tryb�w.");
        return { 1920, 1080 }; // Domy�lne warto�ci rozdzielczo�ci
    }

    // Przyk�adowo wybieramy pierwszy dost�pny tryb
    SDL_DisplayMode* mode = modes[0];
    if (mode == nullptr)
	{
        SDL_Log("Pierwszy tryb wy�wietlania jest nieprawid�owy.");
        SDL_free(modes);
        return { 1920, 1080 }; // Domy�lne warto�ci rozdzielczo�ci
    }

    std::pair<float, float> windowSize = { mode->w, mode->h };

    // Zwolnienie pami�ci przydzielonej przez SDL_GetFullscreenDisplayModes
    SDL_free(modes);

    return windowSize;
}

auto [width, height] = GetWindowSize();

float DEFAULT_WIDTH = width;
float DEFAULT_HEIGHT = height;

float scaleX = WINDOW_WIDTH / width;
float scaleY = WINDOW_HEIGHT / height;

// Delay = 1000 / fps

// 30 - 33.33f
// 60 - 16.67f
// 120 - 8.33f
// 144 - 6.94f
// 240 - 4.17f

std::vector<float> availableFPS =
{
	30.0f,
	60.0f,
	120.0f, 
	144.0f, 
	240.0f 
};

float fps_t = availableFPS[1]; // Domy�lna ilo�� klatek na sekund�
float fpsf = 1000.0f / fps_t; // Domy�lna ilo�� klatek na sekund�