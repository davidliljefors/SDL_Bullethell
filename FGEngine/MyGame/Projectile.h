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
	Projectile(FG::Sprite* sprite, float lifetime, bool playerFired, FG::Vector2D velocity, FG::Camera* camera);
	Projectile(const Projectile&);
	void Update(float deltaTime) override;
	void Render(FG::Camera* const camera) override;

	virtual SDL_Rect GetColliderRect() override;
	void OnCollision(FG::Entity* other) override;
	bool Expired() { return elapsedTime > lifetime; }

private:
	FG::Camera* camera = nullptr;

	bool isColliding = false;
	bool playerFired;

	float elapsedTime = 0.f;
	float lifetime;
	FG::Vector2D velocity;


	void DrawColliderCircle();
	void DrawBoundingBox();
	void Move(float deltaTime);

	virtual void ProjectileUpdate();
	virtual void OnLifetimeEnd() {}

	SDL_Color notCollidingColor = { 0, 255, 0, 255 };
	SDL_Color CollidingColor = { 255, 0, 0, 255 };
};

