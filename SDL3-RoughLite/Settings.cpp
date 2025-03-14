#include <tuple>
#include "Settings.hpp"
#include <vector>
#include <algorithm>
#include "SDL3/SDL.h"

float fps_t = 60.0f;

std::vector<Resolution> GetAvailableResolutions()
{
    std::vector<Resolution> resolutions;
    SDL_DisplayID display = SDL_GetPrimaryDisplay();

    if (display == 0)
    {
        SDL_Log("Nie uda�o si� pobra� g��wnego wy�wietlacza: %s", SDL_GetError());
        return resolutions;
    }

    int num_modes = 0;
    SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(display, &num_modes);
    if (modes == nullptr || num_modes == 0)
    {
        SDL_Log("Nie uda�o si� pobra� tryb�w wy�wietlania lub brak dost�pnych tryb�w: %s", SDL_GetError());
        return resolutions; // Zwraca pusty wektor w przypadku b��du
    }

    for (int i = 0; i < num_modes; ++i)
    {
        SDL_DisplayMode* mode = modes[i];
        if (mode != nullptr)
        {
            Resolution res = { mode->w, mode->h };
            float freshRate = mode->refresh_rate;
            // Sprawdzamy, czy rozdzielczo�� ju� istnieje w wektorze
            if (std::find_if(resolutions.begin(), resolutions.end(),
                [&res](const Resolution& r) { return r.width == res.width && r.height == res.height; }) == resolutions.end())
            {
                resolutions.push_back(res);
                SDL_Log("Dodano rozdzielczo��: %dx%d", res.width, res.height);
            }
        }
    }

    // Zwolnienie pami�ci przydzielonej przez SDL_GetFullscreenDisplayModes
    SDL_free(modes);

    return resolutions;
}

std::vector<float> GetAvailableFPS()
{
	std::vector<float> fps;
    SDL_DisplayID display = SDL_GetPrimaryDisplay();

    if (display == 0)
    {
        SDL_Log("Nie uda�o si� pobra� g��wnego wy�wietlacza: %s", SDL_GetError());
        return fps;
    }

    int num_modes = 0;
    SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(display, &num_modes);
    if (modes == nullptr || num_modes == 0)
    {
        SDL_Log("Nie uda�o si� pobra� tryb�w wy�wietlania lub brak dost�pnych tryb�w: %s", SDL_GetError());
        return fps; // Zwraca pusty wektor w przypadku b��du
    }

    for (int i = 0; i < num_modes; ++i)
    {
        SDL_DisplayMode* mode = modes[i];
        if (mode != nullptr)
        {
            float freshRate = mode->refresh_rate;
            // Sprawdzamy, czy rozdzielczo�� ju� istni

			if (std::find(fps.begin(), fps.end(), freshRate) == fps.end())
			{
				fps.push_back(freshRate);
			}
        }
    }

    // Zwolnienie pami�ci przydzielonej przez SDL_GetFullscreenDisplayModes
    SDL_free(modes);

    return fps;
}

std::vector<Resolution> availableResolutions;
Resolution currentResolution = { 1920, 1080 }; // Domy�lna rozdzielczo��

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

std::vector<float> availableFPS;

void InitializeSettings()
{
    availableResolutions = GetAvailableResolutions();
    if (!availableResolutions.empty())
    {
        currentResolution = availableResolutions[0];
    }

	availableFPS = GetAvailableFPS();
	if (!availableFPS.empty())
	{
        fps_t = availableFPS[0];
	}

}

float fpsf = 1000.0f / fps_t; // Domy�lna ilo�� klatek na sekund�