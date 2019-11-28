#include "Healthbar.h"
#include "Obstacle.h"
#include <Camera.h>
#include <SDL_render.h>


void Healthbar::Render(FG::Camera* camera)
{
	if (!isActive)
	{
		return;
	}
	SDL_Renderer* r = camera->GetInternalRenderer();
	SDL_Rect rect = 	{
	static_cast<int>(position.x) - 2,
	static_cast<int>(position.y) - 2,
	static_cast<int>(boss->GetHealth() / static_cast<float>(boss->GetMaxHealth())* size.x + 4),
	static_cast<int>(size.y + 4) 
	};

	SDL_SetRenderDrawColor(r, 166, 0, 0, 0);
	SDL_RenderFillRect(r, &rect);

	rect = 	{
		static_cast<int>(position.x),
		static_cast<int>(position.y),
		static_cast<int>(boss->GetHealth() / static_cast<float>(boss->GetMaxHealth())* size.x),
		static_cast<int>(size.y) 
	};
	SDL_SetRenderDrawColor(r, 232, 0, 0, 0);
	SDL_RenderFillRect(r, &rect);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
}

void Healthbar::SetActive(bool state)
{
	isActive = state;
}
