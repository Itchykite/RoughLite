#include "Player.hpp"
#include "Enemy.hpp"
#include "Settings.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <SDL3_image/SDL_image.h>

Player::Player(Map* map, Camera* camera, SDL_Renderer* renderer) // Konstruktor gracza, mapa, kamera, renderer
    : map(map), camera(camera), x(0), y(0), velocityX(0), velocityY(0),
    playerTexture(nullptr), attackTexture(nullptr), frameWidth(0), frameHeight(0),
    currentFrame(0), totalFrames(0), lastFrameTime(0), frameDuration(100),
    attackFrameDuration(10), // Inicjalizacja zmiennej attackFrameDuration
	currentRow(0), isAttacking(false), attackFrame(0), attackRow(0), kills(0), texture(nullptr), health(100), isGameOver(false)
{
    SDL_Log("Loading player texture...");
    LoadTexture(renderer, "spritesheet.png"); // Wczytanie tekstury gracza
    SDL_Log("Loading attack texture...");
    LoadAttackTexture(renderer, "Images/attack.png"); // Wczytanie tekstury ataku

    TTF_Init();

    // Za³adowanie czcionki
    font = TTF_OpenFont("Poppins-Bold.ttf", 24);
    if (!font) 
    {
        SDL_Log("Failed to load font: %s", SDL_GetError());
    }
}

Player::~Player() // Destruktor
{
    if (playerTexture) // Jeœli tekstura gracza istnieje
    {
        SDL_Log("Destroying player texture...");
        SDL_DestroyTexture(playerTexture); // Usuniêcie tekstury gracza
        playerTexture = nullptr; // Ustawienie tekstury gracza na nullptr
    }
    if (attackTexture) // Jeœli tekstura ataku istnieje
    {
        SDL_Log("Destroying attack texture...");
        SDL_DestroyTexture(attackTexture); // Usuniêcie tekstury ataku
        attackTexture = nullptr; // Ustawienie tekstury ataku na nullptr
    }
    if (font)
    {
        TTF_CloseFont(font);
        font = nullptr;
    }
    if (texture) 
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    TTF_Quit();
}

void Player::HandleCollision() // Obs³uga kolizji
{
	if (!map->IsWithinBounds(x, y, 64, 64)) // Jeœli gracz nie jest w granicach
    {
		x -= velocityX; // Ustawienie pozycji x
		y -= velocityY; // Ustawienie pozycji y
    }
}

void Player::Update(float deltaTime) // Aktualizacja gracza
{
    float newX = x + velocityX * deltaTime; // Nowa pozycja x
    float newY = y + velocityY * deltaTime; // Nowa pozycja y

    if (map->IsWithinBounds(newX, newY + (3.0f * playerH / 4.0f), playerW, playerH / 4.0f)) // Jeœli gracz jest w granicach
    {
        x = newX; // Ustaw pozycjê x
        y = newY; // Ustaw pozycjê y
    }

    if (velocityX == 0 && velocityY == 0) // Jeœli prêdkoœæ x i y s¹ równe 0
    {
        currentFrame = 0; // Ustaw klatkê na 0
    }
    else if (velocityX > 0) // Right
    {
        currentRow = 3; // Ustaw klatkê na 3
    }
    else if (velocityX < 0) // Left
    {
        currentRow = 1; // Ustaw klatkê na 1
    }
    else if (velocityY > 0) // Up
    {
        currentRow = 2; // Ustaw klatkê na 2
    }
    else if (velocityY < 0) // Down
    {
        currentRow = 0; // Ustaw klatkê na 0
    }

    if (camera != nullptr) // Jeœli kamera istnieje
    {
        camera->Update(x, y); // Aktualizacja kamery
    }
    else
    {
        SDL_Log("Camera is not initialized!");
    }

    UpdateAnimation(); // Aktualizacja animacji
}

void Player::SetCurrentRow(int row) // Ustawienie aktualnego wiersza dla animacji
{
	currentRow = row; // Ustaw wiersz
}

void Player::LoadTexture(SDL_Renderer* renderer, const char* pathFile) // Wczytanie tekstury gracza
{
    SDL_Surface* surface = IMG_Load(pathFile); // Wczytanie powierzchni gracza
    if (!surface) // Jeœli powierzchnia gracza nie istnieje
    {
        SDL_Log("Couldn't load player texture: %s", SDL_GetError());
        return;
    }

    playerTexture = SDL_CreateTextureFromSurface(renderer, surface); // Utworzenie tekstury gracza
    frameWidth = surface->w / 9; // Szerokoœæ klatki
    frameHeight = surface->h / 4; // Wysokoœæ klatki
    totalFrames = 4; // Ca³kowita liczba klatek

    SDL_DestroySurface(surface); // Usuniêcie powierzchni
}

void Player::LoadAttackTexture(SDL_Renderer* renderer, const char* pathFile) // Wczytanie tekstury ataku
{
    SDL_Surface* surface = IMG_Load(pathFile); // Wczytanie powierzchni ataku
    if (!surface) // Jeœli powierzchnia ataku nie istnieje
    {
        SDL_Log("Couldn't load attack texture: %s", SDL_GetError());
        return;
    }

    attackTexture = SDL_CreateTextureFromSurface(renderer, surface); // Utworzenie tekstury ataku
    SDL_DestroySurface(surface); // Usuniêcie powierzchni
}

void Player::UpdateAnimation() // Aktualizacja animacji
{
    Uint64 currentTime = SDL_GetTicks(); // Aktualny czas
    if (currentTime > lastFrameTime + frameDuration) // Je¿eli aktualny czas jest wiêkszy od ostatniego czasu klatki + czasu trwania klatki
    {
        currentFrame = (currentFrame + 1) % totalFrames; // Ustaw klatkê
        lastFrameTime = currentTime; // Ustaw ostatni czas klatki
    }

    if (isAttacking)
    {
        if (currentTime > lastFrameTime + attackFrameDuration) // U¿yj attackFrameDuration dla animacji ataku
        {
            attackFrame = (attackFrame + 1) % 6; // Aktualizacja klatki ataku
            lastFrameTime = currentTime; // Ustaw ostatni czas klatki ataku
            if (attackFrame == 0)
            {
                isAttacking = false; // Zakoñczenie ataku
            }
        }
    }
}

void Player::renderHealthBar(double healthValue, SDL_Renderer* renderer) // Renderowanie paska zdrowia
{
	float barHeight = 25.0f; // Wysokoœæ paska
	float barWidth = WINDOW_WIDTH; // Szerokoœæ paska

	float healthPercentage = healthValue / 100.0f; // Procent zdrowia
	float currentBarWidth = barWidth * healthPercentage; // Aktualna szerokoœæ paska

	float x = (WINDOW_WIDTH - barWidth); // Pozycja x
	float y = (WINDOW_HEIGHT - barHeight); // Pozycja y

	SDL_FRect backgroundBar = { x, y, barWidth, barHeight };
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	SDL_RenderFillRect(renderer, &backgroundBar);

	SDL_FRect foregroundBar = { x, y, currentBarWidth, barHeight };
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &foregroundBar);
}

void Player::Render(SDL_Renderer* renderer) // Renderowanie gracza
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Ustaw kolor renderowania
    SDL_FRect player = { x - camera->GetX(), y - camera->GetY(), playerW, playerH }; // Ustawienie pozycji gracza
    SDL_FRect srcRect = { static_cast<int>(currentFrame * frameWidth), static_cast<int>(currentRow * frameHeight), static_cast<int>(frameWidth), static_cast<int>(frameHeight) }; // Ustawienie klatki

    SDL_RenderTexture(renderer, playerTexture, &srcRect, &player); // Renderowanie tekstury gracza

    if (isAttacking)
    {
        float attackOffsetX = 0.0f;
        float attackOffsetY = 0.0f;

        // Oblicz przesuniêcie animacji ataku w zale¿noœci od kierunku ataku
        switch (attackRow)
        {
        case 0: // Right
            attackOffsetX = playerW / 1.5f;
            break;
        case 1: // Left
            attackOffsetX = -playerW / 1.5f;
            break;
        case 2: // Down
            attackOffsetY = playerH / 1.5f;
            break;
        case 3: // Up
            attackOffsetY = -playerH / 1.5f;
            break;
        case 4: // Right Down
            attackOffsetX = playerW / 1.5f;
            attackOffsetY = playerH / 1.5f;
            break;
        case 5: // Right Up
            attackOffsetX = playerW / 1.5f;
            attackOffsetY = -playerH / 1.5f;
            break;
        case 6: // Left Down
            attackOffsetX = -playerW / 1.5f;
            attackOffsetY = playerH / 1.5f;
            break;
        case 7: // Left Up
            attackOffsetX = -playerW / 1.5f;
            attackOffsetY = -playerH / 1.5f;
            break;
        default:
            break;
        }

        SDL_FRect attackSrcRect = { static_cast<int>(attackFrame * 64), static_cast<int>(attackRow * 64), 64, 64 }; // Ustawienie klatki ataku
        SDL_FRect attackDestRect = { player.x + attackOffsetX, player.y + attackOffsetY, playerW, playerH }; // Ustawienie pozycji animacji ataku z rozmiarem 128x128
        SDL_RenderTexture(renderer, attackTexture, &attackSrcRect, &attackDestRect); // Renderowanie tekstury ataku
    }

    if (texture)
    {
        float w = 0, h = 0;
        SDL_GetTextureSize(texture, &w, &h);

        SDL_FRect dst;
        dst.w = w;
        dst.h = h;
        dst.x = (WINDOW_WIDTH - w) / 2;
        dst.y = (WINDOW_HEIGHT * 0.02f);

        SDL_RenderTexture(renderer, texture, NULL, &dst);
    }
}

void Player::SetPosition(float x, float y) 
{
	this->x = x; // Ustaw pozycjê x
	this->y = y; // Ustaw pozycjê y
}

void Player::SetVelocity(float x, float y)
{
	this->velocityX = x; // Ustaw prêdkoœæ x
	this->velocityY = y; // Ustaw prêdkoœæ y
}

float Player::GetX() const
{
    return x;
}

float Player::GetY() const
{
    return y;
}

float Player::GetVelocityX() const
{
    return velocityX;
}

float Player::GetVelocityY() const
{
    return velocityY;
}

float Player::GetHeight() const
{
    return static_cast<float>(frameHeight);
}

// Funkcja sprawdzaj¹ca, czy wróg zosta³ trafiony
bool isEnemyHit(float playerX, float playerY, float dirX, float dirY, Enemy& enemy, float attackRange, float attackAngle) 
{
    float playerCenterX = playerX + playerW / 16.0f;
    float playerCenterY = playerY + playerH / 16.0f;

    float enemyVecX = (enemy.GetX() + enemyW / 2.0f - playerCenterX) + playerW / 16 - 32.0f;
    float enemyVecY = (enemy.GetY() + enemyH / 2.0f - playerCenterY) + playerH / 16 - 32.0f;

    float distance = std::sqrt(enemyVecX * enemyVecX + enemyVecY * enemyVecY);

    if (distance > attackRange) return false;

    // Normalizacja kierunku ataku i wektora do wroga
    float length = std::sqrt(dirX * dirX + dirY * dirY);
    if (length > 0.001f) // Unikamy dzielenia przez 0
    {
        dirX /= length;
        dirY /= length;
    }

    float enemyDirX = enemyVecX / distance;
    float enemyDirY = enemyVecY / distance;

    // Iloczyn skalarny
    float dot = dirX * enemyDirX + dirY * enemyDirY;
    float angle = std::acos(dot) * 180.f / 3.14159f;

    return angle < attackAngle / 2.f;
}

void Player::attack(SDL_Renderer* renderer, std::vector<std::unique_ptr<Enemy>>& enemies, float dirX, float dirY) // Atak
{
    isAttacking = true;
    attackFrame = 0;

    if (dirX > 0 && dirY == 0) // Right
    {
        attackRow = 0;
    }
    else if (dirX < 0 && dirY == 0) // Left
    {
        attackRow = 1;
    }
    else if (dirX == 0 && dirY > 0) // Down
    {
        attackRow = 2;
    }
    else if (dirX == 0 && dirY < 0) // Up
    {
        attackRow = 3;
    }
    else if (dirX > 0 && dirY > 0) // Right Down
    {
        attackRow = 4;
    }
    else if (dirX > 0 && dirY < 0) // Right Up
    {
        attackRow = 5;
    }
    else if (dirX < 0 && dirY > 0) // Left Down
    {
        attackRow = 6;
    }
    else if (dirX < 0 && dirY < 0) // Left Up
    {
        attackRow = 7;
    }
    float attackRange = 200.0f;
    float attackAngle = 100.0f;

    for (auto& enemy : enemies)
    {
        if (isEnemyHit(x, y, dirX, dirY, *enemy, attackRange, attackAngle))
        {
            enemy->health -= 100;
            if (enemy->health <= 0)
            {
                enemy->isAlive = false;
                kills++; // Zwiêkszenie liczby zabójstw
                UpdateKillsTexture(renderer); // Aktualizacja tekstury z liczb¹ zabójstw
                enemy->~Enemy();
            }
        }
    }
}

void Player::UpdateKillsTexture(SDL_Renderer* renderer)
{
    if (texture) 
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    std::string killsText = "Kills: " + std::to_string(kills);
    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, killsText.c_str(), 0, textColor);
    if (!textSurface) {
        SDL_Log("Unable to render text surface: %s", SDL_GetError());
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!texture) {
        SDL_Log("Unable to create texture from rendered text: %s", SDL_GetError());
    }

    SDL_DestroySurface(textSurface);
}

void GameOver(SDL_Renderer* renderer, TTF_Font* font, Player* player) // Funkcja wyœwietlaj¹ca napis Game Over
{
    SDL_Color textColor = { 255, 255, 255, 255 };

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Game Over", 0, textColor);
    if (!textSurface)
    {
        SDL_Log("Unable to render text surface: %s", SDL_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!texture)
    {
        SDL_Log("Unable to create texture from rendered text: %s", SDL_GetError());
    }

    SDL_FRect dst;
    dst.w = textSurface->w;
    dst.h = textSurface->h;
    dst.x = (WINDOW_WIDTH - dst.w) / 2;
    dst.y = (WINDOW_HEIGHT - dst.h) / 2;
    SDL_RenderTexture(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(textSurface);

    player->isGameOver = true;
}

SDL_FRect Player::GetCollisionRect() const // Pobranie prostok¹ta kolizji
{
    float collisionHeight = playerH / 4.0f; // Wysokoœæ kolizji (np. 1/4 wysokoœci gracza)
    return SDL_FRect
    {
        x,                          // X
        y + (3.0f * playerH / 4.0f),// Y (przesuniêcie w dó³)
        playerW,                    // Szerokoœæ
        collisionHeight             // Wysokoœæ
    };
}