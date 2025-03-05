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