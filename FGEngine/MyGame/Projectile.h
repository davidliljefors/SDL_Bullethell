#pragma once

#include <Entity.h>
#include <Vector2D.h>

namespace FG
{
	class Window;
	class Camera;
	class Sprite;
}

enum ProjectileType
{
	Regular,
	SpeedWave
};

class Projectile : public FG::Entity
{
public:


	Projectile(float speed, float lifetime, bool playerFired, float scale, FG::Vector2D direction, FG::Camera* camera);
	Projectile(float minSpeed, float maxSpeed, float lifetime, bool playerFired, float scale, FG::Vector2D direction, FG::Camera* camera);

	virtual void Update(float deltaTime) override;
	void Render(FG::Camera* const camera) override;

	virtual SDL_Rect GetColliderRect() override;
	void OnCollision(FG::Entity* other) override;

private:
	FG::Camera* camera = nullptr;

	ProjectileType type;

	bool isColliding = false;
	bool playerFired;

	float speed;
	float lifetime;
	float maxLifetime;
	float scale;

	float minSpeed;
	float maxSpeed;

	FG::Vector2D direction;

	void DrawBoundingBox();
	void Move(float deltaTime);

	virtual void OnLifetimeEnd() = 0;

	SDL_Color notCollidingColor = { 0, 255, 0, 255 };
	SDL_Color CollidingColor = { 255, 0, 0, 255 };
};

