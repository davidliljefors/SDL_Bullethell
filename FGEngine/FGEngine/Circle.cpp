#include "Circle.h"
#include "Camera.h"

void FG::Circle::Draw(Camera* camera, unsigned char r,unsigned char g,unsigned char b)
{

	static const int samples = 100;
	static bool generateUnitCircle = true;
	static FG::Vector2D positions[samples + 1];
	if (generateUnitCircle)
	{
		for (int i = 0; i < samples + 1; i++)
		{
			positions[i].x = sin(360.f / samples * i * 3.14159f / 180.f);
			positions[i].y = cos(360.f / samples * i * 3.14159f / 180.f);
		}
		generateUnitCircle = false;
	}

	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), r, g, b, 255);

	for (int i = 0; i < samples; i++)
	{
		SDL_RenderDrawLine(camera->GetInternalRenderer(),
			(int)(positions[i].x*radius+position->x), (int)(positions[i].y * radius + position->y), 
			(int)(positions[i + 1].x*radius+position->x), (int)(positions[i + 1].y * radius + position->y));
	}

	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), 0, 0, 0, 255);
}
