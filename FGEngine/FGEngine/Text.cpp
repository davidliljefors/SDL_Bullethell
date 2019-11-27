#include "Camera.h"

#include "Text.h"
#include "SDL_ttf.h"

void FG::Text::Dispose()
{
	if (texture)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}

bool FG::Text::SetText(SDL_Renderer* renderer, std::string text, const std::string& path, int textSize, SDL_Color color)
{
	Dispose();
	TTF_Font* font = TTF_OpenFont(path.c_str(), textSize);
	if (font == nullptr) {
		printf("Font Loading error: Font-%s Error-%s\n", (path).c_str(), TTF_GetError());
		return false;
	}
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (surface == NULL) {
		printf("Text Render Error: %s\n", TTF_GetError());
		return false;
	}
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL) {
		printf("Text Texture Creation Error: %s\n", SDL_GetError());
		return false;
	}
	int width = 0;
	int height = 0;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
	size = { static_cast<float>(width), static_cast<float>(height) };

	SDL_FreeSurface(surface);

	TTF_CloseFont(font);
	return true;
}

void FG::Text::Render(Camera* camera, Vector2D position, const SDL_Rect* src)
{
	Vector2D finalPosition = position - camera->position;
	destination = { static_cast<int>(finalPosition.x - (size.x / 2)), static_cast<int>(finalPosition.y - (size.y / 2)),
	static_cast<int>(size.x), static_cast<int>(size.y) };
	SDL_RenderCopy(camera->GetInternalRenderer(), texture, src, &destination);
}

void FG::Text::Render(Camera* camera, Vector2D position, char alpha, const SDL_Rect* src)
{
	SDL_SetTextureAlphaMod(texture, alpha);
	Render(camera, position, src);
	SDL_SetTextureAlphaMod(texture, 255);
}