#pragma once
#include "Entity.h"
#include "Vector2D.h"
#include <SDL_pixels.h>

namespace FG
{
	class Sprite;
	class Camera;
}

class Obstacle : public FG::Entity
{
public:
	void Update(float deltaTime) override;
	Obstacle(FG::Camera* camera) : camera(camera) {
		collisionLayer.set(1);
		collisionLayer.set(0);
	}
	void Render(FG::Camera* const camera) override;

	SDL_Rect GetColliderRect() override;
	void OnCollision(FG::Entity* other) override;

private:
	bool isColliding = false;
	SDL_Color notCollidingColor = { 0, 255, 0, 255 };
	SDL_Color CollidingColor = { 255, 0, 0, 255 };
	FG::Camera* camera = nullptr;

	void DrawBoundingBox();
	void DrawColliderCircle();
};


