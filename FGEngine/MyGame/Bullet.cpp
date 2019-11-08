#include "Bullet.h"

#include <Camera.h>
#include <SDL_render.h>


Bullet::Bullet(FG::EntityManager* manager, FG::Camera* camera, FG::Vector2D pos, FG::Vector2D vel)
	: FG::Entity(manager), camera(camera), velocity(vel) 
{
	position = pos;
}

Bullet::Bullet(const Bullet& other)
{
	sprite = other.sprite;
	camera = other.camera;
	manager = other.manager;
}

void Bullet::Render(FG::Camera* const camera)
{
	Entity::Render(camera);
	//camera->DrawSprite(sprite, src, dst);
}

void Bullet::Update(float deltaTime)
{
	position += velocity;
	lifetime -= deltaTime;
	if (lifetime < 0)
	{
		this->Destroy();
	}
}
