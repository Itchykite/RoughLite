#include "Map.hpp"
#include "Player.hpp"

Map::Map(int width, int height) : width(width), height(height) // Konstruktor mapy, szerokoœæ, wysokoœæ
{
	CreateCollisionSurface(); // Utworzenie powierzchni kolizji
}

void Map::CreateCollisionSurface() // Utworzenie powierzchni kolizji
{
	collisionSurface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32); // Utworzenie powierzchni kolizji
	if (!collisionSurface) // Jeœli powierzchnia kolizji nie istnieje
    {
        SDL_Log("Unable to create collision surface! SDL Error: %s", SDL_GetError());
    }
}

bool Map::IsWithinBounds(float x, float y, float px, float py) const // Czy jest w granicach
{
	if (x < 64.0f || x >(width - px - 64.0f) || y < 0.0f || y >(height - py - 64.0f)) // Jeœli x jest mniejsze od 64, x jest wiêksze od szerokoœci - px - 64, y jest mniejsze od 0, y jest wiêksze od wysokoœci - py - 63
    {
        return false;
    }

	SDL_FRect playerRect = { x, y, px, py }; // Prostok¹t gracza
	for (const auto& rect : collisionRects) // Dla ka¿dego prostok¹ta kolizji
    {
		if (SDL_HasRectIntersectionFloat(&playerRect, &rect)) // Jeœli jest kolizja
		{
			return false;
		}
    }

    return true;
}

bool Map::LoadTexture(SDL_Renderer* renderer, const char* pathFile) // Wczytanie tekstury
{ 
	backgroundTexture = IMG_LoadTexture(renderer, pathFile); // Wczytanie tekstury
	return backgroundTexture != nullptr; // Zwróæ czy tekstura istnieje
}

void Map::Render(SDL_Renderer* renderer, float cameraX, float cameraY) const // Renderowanie mapy    
{
	if (backgroundTexture) // Jeœli tekstura t³a istnieje
    {
		SDL_FRect srcRect = { 0, 0, width, height }; // Prostok¹t Ÿród³owy
		SDL_FRect dstRect = { -cameraX, -cameraY, width, height }; // Prostok¹t docelowy
		SDL_RenderTexture(renderer, backgroundTexture, &srcRect, &dstRect); // Renderowanie tekstury
    }
}

bool Map::IsNodeWalkable(int x, int y) const // Czy wêze³ jest dostêpny
{
	if (x < 0 || x >= width / nodeSize || y < 0 || y >= height / nodeSize) // Jeœli x jest mniejsze od 0, x jest wiêksze od szerokoœci / rozmiar wêz³a, y jest mniejsze od 0, y jest wiêksze od wysokoœci / rozmiar wêz³a
    {
		return false; // Zwróæ fa³sz
    }

	return grid[y][x]; // Zwróæ siatkê
}

void Map::CreateGrid(int nodeSize) // Utworzenie siatki
{
	this->nodeSize = nodeSize; // Ustawienie rozmiaru wêz³a
	int rows = height / nodeSize; // Wiersze
    int cols = width / nodeSize; // Koluny
	grid.resize(rows, std::vector<bool>(cols, true)); // Zmiana rozmiaru siatki
}

void Map::AddCollisionRect(const SDL_FRect& rect) // Dodanie prostok¹ta kolizji
{
	collisionRects.push_back(rect); // Dodanie prostok¹ta kolizji
}

bool Map::LoadCollisionSurface(const char* pathFile) // Wczytanie powierzchni kolizji
{
	SDL_Surface* loadedSurface = IMG_Load(pathFile); // Wczytanie powierzchni kolizji
	if (!loadedSurface) // Jeœli powierzchnia kolizji nie istnieje
	{
		SDL_Log("Unable to load collision surface %s! SDL_image Error: %s", pathFile, SDL_GetError()); // Wyœwietlenie b³êdu
		return false; // Zwróæ fa³sz
	}

	SDL_Surface* scaledSurface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32); // Utworzenie powierzchni o szerokoœci, wysokoœci, formacie
	if (!scaledSurface) // Jeœli powierzchnia nie istnieje
    {
        SDL_Log("Unable to create scaled surface! SDL Error: %s", SDL_GetError());
		SDL_DestroySurface(loadedSurface); // Usuniêcie powierzchni
        return false;
    }

	SDL_BlitSurfaceScaled(loadedSurface, nullptr, scaledSurface, nullptr, SDL_SCALEMODE_NEAREST); // Skalowanie powierzchni
	SDL_DestroySurface(loadedSurface); // Usuniêcie powierzchni

	if (collisionSurface) // Jeœli powierzchnia kolizji istnieje
    {
		SDL_DestroySurface(collisionSurface); // Usuniêcie powierzchni kolizji
    }
	collisionSurface = scaledSurface; // Ustawienie powierzchni kolizji

    return true;
}

bool Map::IsPixelTransparent(int x, int y) const // Sprawdzenie czy pixel jest przezroczysty
{
    y += 128;
    x += 64;

	if (!collisionSurface || x < 0 || x >= collisionSurface->w || y < 0 || y >= collisionSurface->h) // Jeœli powierzchnia kolizji nie istnieje, x jest mniejsze od 0, x jest wiêksze od szerokoœci powierzchni kolizji, y jest mniejsze od 0, y jest wiêksze od wysokoœci powierzchni kolizji
    {
        return true;
    }

	Uint32* pixels = (Uint32*)collisionSurface->pixels; // Piksele
	Uint32 pixel = pixels[(y * collisionSurface->w) + x]; // Piksel
    

	Uint8 r, g, b, a; // Kana³y
	SDL_GetRGBA(pixel, SDL_GetPixelFormatDetails(collisionSurface->format), 0, &r, &g, &b, &a); // Ustawienie kolizji na podsatwie kana³ów

    return a == 0; 
}

void mapCollisions(Map* map)
{
	//map->AddCollisionRect({ 512, 512, 64, 64 });
}