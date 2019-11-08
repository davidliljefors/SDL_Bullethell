#pragma once

#include <Entity.h>
#include <Vector2D.h>
#include "SDL.h"


namespace FG
{
class EntityManager;
	class Window;
	class InputManager;
	class Camera;
}

class Player : public FG::Entity
{
public:
	Player(FG::EntityManager* manager, FG::InputManager* inputManager, FG::Camera* camera);

	void Update(float deltaTime);
	void Render(FG::Camera* const camera);
	void Shoot() override;
	void Collided(Entity& other) override
	{
		other.Destroy();
	}

	float playerSpeed = 150.0f;

private:
	FG::InputManager* inputManager = nullptr;
	FG::Camera* camera = nullptr;

	Player() = delete;

	float fireSpeed = 0.15f;
	float shotDelay = 0.f;

	void MovePlayer(float deltaTime);
	void MoveCamera(float deltaTime);
};
