#include "Healthbar.h"
#include "Obstacle.h"
#include <Camera.h>
#include <SDL_render.h>


void Healthbar::Render(FG::Camera* camera)
{
	SDL_Renderer* r = camera->GetInternalRenderer();
	SDL_Rect rect = { static_cast<int>(position.x), 
		static_cast<int>(position.y), 
		static_cast<float>(boss->GetHealth()) / 
		static_cast<float>(boss->GetMaxHealth()) * 
		static_cast<int>(size.x), 
		static_cast<int>(size.y) };
	SDL_SetRenderDrawColor(r, 255, 0, 0, 0);
	SDL_RenderFillRect(r, &rect);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
}
