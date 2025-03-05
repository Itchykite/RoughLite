#include "Settings.hpp"

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

Resolution currentResolution = availableResolutions[12]; // Domyœlna rozdzielczoœæ

float WINDOW_WIDTH = currentResolution.width;
float WINDOW_HEIGHT = currentResolution.height;

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

float fps_t = availableFPS[1]; // Domyœlna iloœæ klatek na sekundê
float fpsf = 1000.0f / fps_t; // Domyœlna iloœæ klatek na sekundê