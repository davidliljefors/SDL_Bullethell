#pragma once

#include <Entity.h>
#include <Vector2D.h>
#include <vector>


namespace FG
{
	class EntityManager;
	class Window;
	class Camera;
}

class Enemy : public FG::Entity
{
public:
	Enemy(FG::EntityManager& manager, FG::Camera* camera, FG::Vector2D pos);
	void Update(float deltaTime);
	void Render(FG::Camera* const camera);
	void Collided(Entity& other) override
	{
	}

private:
	
	FG::Camera* camera = nullptr;

	void Shoot();
	float fireSpeed = 1.0f;
	float shotDelay = 0.f;

	std::vector<Entity*> children;

	Enemy() = delete;
};