#include "OtherFunctions.hpp"
#include "Settings.hpp"
#include <string>

//void gameTime(Uint64& startTime, TTF_Font* font, SDL_Texture* textTexture, SDL_Surface* textSurface, SDL_Renderer* renderer)
//{
//	Uint64 currentTime = SDL_GetTicks();
//	Uint64 elapsedTime = (currentTime - startTime) / 1000;
//	std::string timeText = "Time: " + std::to_string(elapsedTime) + " seconds";
//	textSurface = TTF_RenderText_Solid(font, timeText.c_str(), textColor);
//	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
//	SDL_DestroySurface(textSurface);
//}

void RenderGameOverScreen(SDL_Renderer* renderer, Player* player, Uint64 startTime)
{
    // Ustaw kolor t³a na czarny
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Ustaw kolor tekstu na bia³y
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Renderowanie napisu "Game Over"
    TTF_Font* font = TTF_OpenFont("Poppins-Bold.ttf", 48);
    if (!font)
    {
        SDL_Log("Failed to load font: %s", SDL_GetError());
        return;
    }

    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Game Over", 0, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    float textWidth = textSurface->w;
    float textHeight = textSurface->h;
    SDL_FRect textRect = { (WINDOW_WIDTH - textSurface->w) / 2, WINDOW_HEIGHT / 4, textWidth, textHeight };
    SDL_RenderTexture(renderer, textTexture, NULL, &textRect);

    SDL_DestroySurface(textSurface);
    SDL_DestroyTexture(textTexture);

    // Renderowanie iloœci zabójstw
    std::string killsText = "Kills: " + std::to_string(player->kills);
    textSurface = TTF_RenderText_Solid(font, killsText.c_str(), 0, textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    textRect = { (WINDOW_WIDTH - textWidth) / 2, WINDOW_HEIGHT / 2, textWidth, textHeight };
    SDL_RenderTexture(renderer, textTexture, NULL, &textRect);

    SDL_DestroySurface(textSurface);
    SDL_DestroyTexture(textTexture);

    // Renderowanie czasu spêdzonego w grze
    Uint64 currentTime = SDL_GetTicks();
    Uint64 elapsedTime = (currentTime - startTime) / 1000;
    std::string timeText = "Time: " + std::to_string(elapsedTime) + " seconds";
    textSurface = TTF_RenderText_Solid(font, timeText.c_str(), 0, textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    textRect = { (WINDOW_WIDTH - textWidth) / 2, (WINDOW_HEIGHT * 3) / 4, textWidth, textHeight };
    SDL_RenderTexture(renderer, textTexture, NULL, &textRect);

    SDL_DestroySurface(textSurface);
    SDL_DestroyTexture(textTexture);

    TTF_CloseFont(font);
    SDL_RenderPresent(renderer);
}
