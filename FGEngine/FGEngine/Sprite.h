#pragma once
#include "IResource.h"
#include "Vector2D.h"

#include <string>

struct SDL_Texture;
struct SDL_Renderer;

namespace FG
{
	class Camera;

	class Sprite : public IResource
	{
	public:
		Vector2D size;

		void Dispose() override;
		bool LoadImage(SDL_Renderer* renderer, const std::string& path);
		void Render(Camera* camera, Vector2D& position);
		void Render(Camera* camera, Vector2D& position, char alpha);

	protected:
		SDL_Texture* texture = nullptr;
	};
}

