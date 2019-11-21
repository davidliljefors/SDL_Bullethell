#pragma once
#include "Entity.h"
#include "Vector2D.h"
#include <SDL_pixels.h>
#include "Projectile.h"
#include "ProjectilePool.h"
#include <vector>

namespace FG
{
	class Sprite;
	class Camera;
}

enum class Phase
{
	first,
	second,
	third,
	dead
};

class Obstacle : public FG::Entity
{
public:
	void Initialize();
	void Update(float deltaTime) override;
	Obstacle(FG::Camera* camera);
	void Render(FG::Camera* const camera) override;
	void EnterNextPhase();


	SDL_Rect GetColliderRect() override;
	void OnCollision(FG::Entity* other) override;
	bool AddSprite(FG::Sprite* spr) override;

	void StartPosition(FG::Vector2D pos);
	void EnterScreen();
	void PlaceOffscreenForEntrance();
	Phase CurrentPhase();
	void SetUp();
private:
	int health = 1;//15;
	Phase phase = Phase::first;
	bool isColliding = false;
	SDL_Color notCollidingColor = { 0, 255, 0, 255 };
	SDL_Color CollidingColor = { 255, 0, 0, 255 };
	FG::Camera* camera = nullptr;
	std::vector<FG::Sprite*> sprites;

	bool entersScreen;
	bool firstBattle = true;

	FG::Vector2D startPosition;

	ProjectilePool* projectilePool = nullptr;

	void DrawBoundingBox();
	void DrawColliderCircle();
	void OnDeath();
};


