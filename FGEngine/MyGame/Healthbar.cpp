#include "Healthbar.h"
#include "Obstacle.h"
#include <Camera.h>
#include <SDL_render.h>
#include <Text.h>
#include <string>


Healthbar::Healthbar(FG::Vector2D position, FG::Vector2D size, Obstacle* boss)
	: position(position), size(size), boss(boss)
{
	textBarsLeft = new Text();
}

Healthbar::~Healthbar()
{
	if (textBarsLeft)
	{
		textBarsLeft->Dispose();
		delete textBarsLeft;
	}
	
}

void Healthbar::Render(FG::Camera* camera)
{
	assert(boss);
	
	if (!isActive)
	{
		return;
	}
	if (healthLastRender < boss->GetHealth())
	{
		textBarsLeft->SetText(camera->GetInternalRenderer(),
			"x" + std::to_string(boss->GetPhaseCount() - boss->GetCurrentPhase()),
			"radiospace.ttf", 32, { 250,250,250 });
	}

	SDL_Renderer* r = camera->GetInternalRenderer();
	SDL_Rect rect = {
	static_cast<int>(position.x) - 2,
	static_cast<int>(position.y) - 2,
	static_cast<int>(boss->GetHealth() / static_cast<float>(boss->GetMaxHealth())* size.x + 4),
	static_cast<int>(size.y + 4)
	};

	SDL_SetRenderDrawColor(r, 166, 0, 0, 0);
	SDL_RenderFillRect(r, &rect);

	rect = {
		static_cast<int>(position.x),
		static_cast<int>(position.y),
		static_cast<int>(boss->GetHealth() / static_cast<float>(boss->GetMaxHealth())* size.x),
		static_cast<int>(size.y)
	};
	SDL_SetRenderDrawColor(r, 232, 0, 0, 0);
	SDL_RenderFillRect(r, &rect);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
	healthLastRender = boss->GetHealth();
	textBarsLeft->Render(camera, FG::Vector2D({ 30 + position.x + size.x, position.y+2 })); // Render text over hpbar
}

void Healthbar::SetActive(bool state)
{
	isActive = state;
}
