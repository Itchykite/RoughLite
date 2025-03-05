#include "Map.hpp"
#include "Player.hpp"
#include <algorithm>

struct Renderable 
{
    float depth; // np. dolna kraw�d� obiektu
    enum Type { PLAYER, OBJECT } type;
    SDL_Texture* texture; // tylko dla obiekt�w
    SDL_FRect rect;       // tylko dla obiekt�w
    Player* player;       // tylko dla gracza
};

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

    SDL_FRect playerRect = { x, y, px, py };

    for (const auto& objectCollision : objectCollisions)
    {
        for (const auto& rect : objectCollision.collisionRects)
        {
            if (SDL_HasRectIntersectionFloat(&playerRect, &rect)) // Je�li jest kolizja
            {
                return false;
            }
        }
    }

    return true;
}

bool Map::LoadTexture(SDL_Renderer* renderer, const char* pathFile) // Wczytanie tekstury
{ 
	backgroundTexture = IMG_LoadTexture(renderer, pathFile); // Wczytanie tekstury
    SDL_SetTextureScaleMode(backgroundTexture, SDL_SCALEMODE_NEAREST);
	return backgroundTexture != nullptr; // Zwr�� czy tekstura istnieje
}

void Map::Render(SDL_Renderer* renderer, float cameraX, float cameraY) const
{
    if (backgroundTexture)
    {
        SDL_FRect srcRect = { 0, 0, width, height };
        SDL_FRect dstRect = { -cameraX, -cameraY, width, height };
        SDL_RenderTexture(renderer, backgroundTexture, &srcRect, &dstRect);
    }
}

void Map::RenderObjects(SDL_Renderer* renderer, float cameraX, float cameraY, Player* player) const
{
    std::vector<Renderable> renderQueue;

    // Dodaj wszystkie obiekty (drzewa) do kolejki
    for (const auto& object : objectCollisions)
    {
        Renderable r;
        r.depth = object.collisionRects[0].y;
        r.type = Renderable::OBJECT;
        r.texture = object.texture;
        r.rect = object.rect;
        renderQueue.push_back(r);
    }

    // Dodaj gracza do kolejki
    Renderable p;
    p.depth = player->GetY() + player->GetHeight() + 32.0f;
    p.type = Renderable::PLAYER;
    p.player = player;
    renderQueue.push_back(p);

    // Sortuj kolejk� renderowania wed�ug warto�ci depth (od najmniejszej do najwi�kszej)
    std::sort(renderQueue.begin(), renderQueue.end(), [](const Renderable& a, const Renderable& b)
        {
            return a.depth < b.depth;
        });

    // Renderuj wszystkie elementy w kolejno�ci
    for (const auto& item : renderQueue)
    {
        if (item.type == Renderable::PLAYER)
        {
            item.player->Render(renderer);
        }
        else if (item.type == Renderable::OBJECT)
        {
            SDL_FRect dstRect = item.rect;
            dstRect.x -= cameraX;
            dstRect.y -= cameraY;
            SDL_RenderTexture(renderer, item.texture, nullptr, &dstRect);
        }
    }

    player->renderHealthBar(player->health, renderer); // Renderowanie paska jako pierwszego
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

void Map::AddObjectWithCollision(SDL_Renderer* renderer, const char* objectTexturePath, float posX, float posY, float objWidth, float objHeight)
{
    SDL_Texture* objectTexture = IMG_LoadTexture(renderer, objectTexturePath);
    if (!objectTexture)
    {
        SDL_Log("Nie mo�na wczyta� tekstury obiektu %s! SDL_image Error: %s", objectTexturePath, SDL_GetError());
        return;
    }

    SDL_SetTextureScaleMode(objectTexture, SDL_SCALEMODE_NEAREST);

    SDL_FRect dstRect = { posX, posY, objWidth, objHeight };

    // Definicja pojedynczego prostok�ta kolizji na dole obiektu
    std::vector<SDL_FRect> collisionRects;

    // Wysoko�� prostok�ta kolizji (1.0f piksel)
    float collisionHeight = 1.0f;

    SDL_FRect collisionRect = {
        posX + objWidth - 126.0f,                   // X
        posY + objHeight - collisionHeight,         // Y
        objWidth - 252.0f,                          // Szeroko��
        collisionHeight                             // Wysoko��
    };

    collisionRects.push_back(collisionRect);

    ObjectCollision objectCollision = { objectTexture, dstRect, collisionRects };
    objectCollisions.push_back(objectCollision);
}

void mapCollisions(Map* map, SDL_Renderer* renderer) // Kolizje mapy
{
    map->AddObjectWithCollision(renderer, "Images/drzewo.png", 512.0f, 512.0f, 256.0f, 256.0f);
    map->AddObjectWithCollision(renderer, "Images/drzewo.png", 620.0f, 948.0f, 256.0f, 256.0f);
    map->AddObjectWithCollision(renderer, "Images/drzewo.png", 2400.0f, 428.0f, 256.0f, 256.0f);
    map->AddObjectWithCollision(renderer, "Images/drzewo.png", 2200.0f, 1852.0f, 256.0f, 256.0f);
    map->AddObjectWithCollision(renderer, "Images/drzewo.png", 1982.0f, 2355.0f, 256.0f, 256.0f);
}

void Map::SortObjectsByY()
{
    std::sort(objectCollisions.begin(), objectCollisions.end(), [](const ObjectCollision& a, const ObjectCollision& b) 
    {
        return a.rect.y < b.rect.y;
    });
}

void Map::Reset(SDL_Renderer* renderer)
{
    // Resetuj map�, np. usu� obiekty kolizji
    objectCollisions.clear();
    // Ponownie utw�rz siatk� kolizji
    CreateGrid(nodeSize);
    // Dodaj obiekty kolizji ponownie, je�li to konieczne
    mapCollisions(this, renderer);
}