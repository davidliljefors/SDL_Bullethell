#pragma once

#include <Entity.h>
#include <Vector2D.h>


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
	float playerSpeed = 150.0f;

	Player(FG::EntityManager& manager, FG::InputManager* inputManager, FG::Camera* camera);
	void Update(float deltaTime);
	void Render(FG::Camera* const camera);

	void Collided(Entity& other) override
	{
		manager.DeleteEntity(&other);
		std::cout << "player col" << std::endl;
	}


private:
	FG::InputManager* inputManager = nullptr;
	FG::Camera* camera = nullptr;

	Player() = delete;

	void MovePlayer(float deltaTime);
	void MoveCamera(float deltaTime);
};
