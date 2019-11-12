#pragma once

#include <Entity.h>
#include <Vector2D.h>

namespace FG
{
	class Window;
	class InputManager;
	class Camera;
	class Sprite;
}

class Player : public FG::Entity
{
public:
	float speed = 80.0f;
	
	Player(FG::InputManager* inputManager, FG::Camera* camera);

	void Update(float deltaTime) override;
	void Render(FG::Camera* const camera) override;


	virtual SDL_Rect GetColliderRect() override;
	void OnCollision(FG::Entity* other) override;

private:
	FG::InputManager* inputManager = nullptr;
	FG::Camera* camera = nullptr;
	bool isColliding = false;
	SDL_Color notCollidingColor = { 0, 255, 0, 255 };
	SDL_Color CollidingColor = { 255, 0, 0, 255 };

	Player() {}

	void DrawBoundingBox();
	void DrawColliderCircle();
	void MovePlayer(float deltaTime);
	void MoveCamera(float deltaTime);
};
