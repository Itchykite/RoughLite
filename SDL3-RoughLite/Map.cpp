#include "Map.hpp"
#include "Player.hpp"

Map::Map(int width, int height) : width(width), height(height) // Konstruktor mapy, szeroko��, wysoko��
{
	CreateCollisionSurface(); // Utworzenie powierzchni kolizji
}

void Map::CreateCollisionSurface() // Utworzenie powierzchni kolizji
{
	collisionSurface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32); // Utworzenie powierzchni kolizji
	if (!collisionSurface) // Je�li powierzchnia kolizji nie istnieje
    {
        SDL_Log("Unable to create collision surface! SDL Error: %s", SDL_GetError());
    }
}

bool Map::IsWithinBounds(float x, float y, float px, float py) const // Czy jest w granicach
{
	if (x < 64.0f || x >(width - px - 64.0f) || y < 0.0f || y >(height - py - 64.0f)) // Je�li x jest mniejsze od 64, x jest wi�ksze od szeroko�ci - px - 64, y jest mniejsze od 0, y jest wi�ksze od wysoko�ci - py - 63
    {
        return false;
    }

	SDL_FRect playerRect = { x, y, px, py }; // Prostok�t gracza
	for (const auto& rect : collisionRects) // Dla ka�dego prostok�ta kolizji
    {
		if (SDL_HasRectIntersectionFloat(&playerRect, &rect)) // Je�li jest kolizja
		{
			return false;
		}
    }

    return true;
}

bool Map::LoadTexture(SDL_Renderer* renderer, const char* pathFile) // Wczytanie tekstury
{ 
	backgroundTexture = IMG_LoadTexture(renderer, pathFile); // Wczytanie tekstury
	return backgroundTexture != nullptr; // Zwr�� czy tekstura istnieje
}

void Map::Render(SDL_Renderer* renderer, float cameraX, float cameraY) const // Renderowanie mapy    
{
	if (backgroundTexture) // Je�li tekstura t�a istnieje
    {
		SDL_FRect srcRect = { 0, 0, width, height }; // Prostok�t �r�d�owy
		SDL_FRect dstRect = { -cameraX, -cameraY, width, height }; // Prostok�t docelowy
		SDL_RenderTexture(renderer, backgroundTexture, &srcRect, &dstRect); // Renderowanie tekstury
    }
}

bool Map::IsNodeWalkable(int x, int y) const // Czy w�ze� jest dost�pny
{
	if (x < 0 || x >= width / nodeSize || y < 0 || y >= height / nodeSize) // Je�li x jest mniejsze od 0, x jest wi�ksze od szeroko�ci / rozmiar w�z�a, y jest mniejsze od 0, y jest wi�ksze od wysoko�ci / rozmiar w�z�a
    {
		return false; // Zwr�� fa�sz
    }

	return grid[y][x]; // Zwr�� siatk�
}

void Map::CreateGrid(int nodeSize) // Utworzenie siatki
{
	this->nodeSize = nodeSize; // Ustawienie rozmiaru w�z�a
	int rows = height / nodeSize; // Wiersze
    int cols = width / nodeSize; // Koluny
	grid.resize(rows, std::vector<bool>(cols, true)); // Zmiana rozmiaru siatki
}

void Map::AddCollisionRect(const SDL_FRect& rect) // Dodanie prostok�ta kolizji
{
	collisionRects.push_back(rect); // Dodanie prostok�ta kolizji
}

bool Map::LoadCollisionSurface(const char* pathFile) // Wczytanie powierzchni kolizji
{
	SDL_Surface* loadedSurface = IMG_Load(pathFile); // Wczytanie powierzchni kolizji
	if (!loadedSurface) // Je�li powierzchnia kolizji nie istnieje
	{
		SDL_Log("Unable to load collision surface %s! SDL_image Error: %s", pathFile, SDL_GetError()); // Wy�wietlenie b��du
		return false; // Zwr�� fa�sz
	}

	SDL_Surface* scaledSurface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32); // Utworzenie powierzchni o szeroko�ci, wysoko�ci, formacie
	if (!scaledSurface) // Je�li powierzchnia nie istnieje
    {
        SDL_Log("Unable to create scaled surface! SDL Error: %s", SDL_GetError());
		SDL_DestroySurface(loadedSurface); // Usuni�cie powierzchni
        return false;
    }

	SDL_BlitSurfaceScaled(loadedSurface, nullptr, scaledSurface, nullptr, SDL_SCALEMODE_NEAREST); // Skalowanie powierzchni
	SDL_DestroySurface(loadedSurface); // Usuni�cie powierzchni

	if (collisionSurface) // Je�li powierzchnia kolizji istnieje
    {
		SDL_DestroySurface(collisionSurface); // Usuni�cie powierzchni kolizji
    }
	collisionSurface = scaledSurface; // Ustawienie powierzchni kolizji

    return true;
}

bool Map::IsPixelTransparent(int x, int y) const // Sprawdzenie czy pixel jest przezroczysty
{
    y += 128;
    x += 64;

	if (!collisionSurface || x < 0 || x >= collisionSurface->w || y < 0 || y >= collisionSurface->h) // Je�li powierzchnia kolizji nie istnieje, x jest mniejsze od 0, x jest wi�ksze od szeroko�ci powierzchni kolizji, y jest mniejsze od 0, y jest wi�ksze od wysoko�ci powierzchni kolizji
    {
        return true;
    }

	Uint32* pixels = (Uint32*)collisionSurface->pixels; // Piksele
	Uint32 pixel = pixels[(y * collisionSurface->w) + x]; // Piksel
    

	Uint8 r, g, b, a; // Kana�y
	SDL_GetRGBA(pixel, SDL_GetPixelFormatDetails(collisionSurface->format), 0, &r, &g, &b, &a); // Ustawienie kolizji na podsatwie kana��w

    return a == 0; 
}

void mapCollisions(Map* map)
{
	//map->AddCollisionRect({ 512, 512, 64, 64 });
}