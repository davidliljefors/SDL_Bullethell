#pragma once

#include <Entity.h>
#include <Vector2D.h>

namespace FG
{
	class Window;
	class Camera;
}

class Enemy : public FG::Entity
{
public:
	Enemy(FG::Camera* camera, FG::Vector2D pos);
	void Update(float deltaTime) {}
	void Render(FG::Camera* const camera);
	FG::Vector2D& GetPosition() override
	{ return position; }
	
private:
	FG::Camera* camera = nullptr;
	FG::Vector2D position;

	Enemy() {}
};