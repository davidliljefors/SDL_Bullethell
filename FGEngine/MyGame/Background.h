#pragma once
#include "Entity.h"
#include "Vector2D.h"
#include <SDL_pixels.h>

namespace FG
{
	class Sprite;
	class Camera;
}

class Background : public FG::Entity
{
public:
	Background(FG::Camera* camera, int speed) : camera(camera), speed(speed) {}
	void Update(float deltaTime) override;

private:

	int speed;

	FG::Camera* camera = nullptr;
};


