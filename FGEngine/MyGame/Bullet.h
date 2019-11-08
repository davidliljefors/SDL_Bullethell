#pragma once


#include <Entity.h>
#include <Vector2D.h>


namespace FG
{
	class EntityManager;
	class Window;
	class Camera;
}

class Bullet : public FG::Entity
{
public:
	Bullet() {}
	Bullet(FG::EntityManager* manager, FG::Camera* camera, FG::Vector2D pos, FG::Vector2D vel);
	Bullet(const Bullet& other);
	void Update(float deltaTime);
	void Render(FG::Camera* const camera);
	void Collided(Entity& other) override
	{
		other.Destroy();
	}

	float lifetime = 3.f;
protected:
	FG::Camera* camera = nullptr;
	FG::Vector2D velocity;

};