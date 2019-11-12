#pragma once

#include <Entity.h>
#include <Vector2D.h>

namespace FG
{
	class Window;
	class Camera;
	class Sprite;
}

class Projectile : public FG::Entity
{
public:
	Projectile(float speed, float lifetime, bool playerFired, FG::Vector2D direction, FG::Camera* camera);

	void Update(float deltaTime) override;
	void Render(FG::Camera* const camera) override;

	virtual SDL_Rect GetColliderRect() override;
	void OnCollision(FG::Entity* other) override;

private:
	FG::Camera* camera = nullptr;

	bool isColliding = false;
	bool playerFired;

	float speed;
	float lifetime;
	FG::Vector2D direction;

	void DrawBoundingBox();
	void Move(float deltaTime);

	virtual void ProjectileUpdate();
	virtual void OnLifetimeEnd() = 0;

	SDL_Color notCollidingColor = { 0, 255, 0, 255 };
	SDL_Color CollidingColor = { 255, 0, 0, 255 };
};

