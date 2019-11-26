#include "Sensor.h"
#include <Camera.h>
#include <SDL_render.h>
#include "Projectile.h"
#include "Player.h"

#include <iostream>

Sensor::Sensor(Player* p_user, SENSOR_TYPE p_type, float p_radius)
{
	m_user = p_user;
	m_type = p_type;
	AddCircleCollider(p_radius);
	collisionLayer.set(6);
	
}

void Sensor::Update(float deltaTime)
{
	 position = m_user->position;
}

void Sensor::Render(FG::Camera* const camera)
{
#ifdef _DEBUG
	assert(collider);
	const int samples = 100;
	SDL_Color color = {0,255,255,0};

	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), color.r, color.g, color.b, color.a);
	FG::Vector2D positions[samples + 1];
	for (int i = 0; i < samples + 1; i++)
	{
		positions[i].x = sin(360.f / samples * i * 3.14159f / 180.f) * collider->GetRadius() + collider->GetPosition().x;
		positions[i].y = cos(360.f / samples * i * 3.14159f / 180.f) * collider->GetRadius() + collider->GetPosition().y;
	}
	for (int i = 0; i < samples; i++)
	{
		SDL_RenderDrawLine(camera->GetInternalRenderer(),
			(int)positions[i].x, (int)positions[i].y, (int)positions[i + 1].x, (int)positions[i + 1].y);
	}

	SDL_SetRenderDrawColor(camera->GetInternalRenderer(), 0, 0, 0, 255);
#endif _DEBUG
}

void Sensor::OnCollision(Entity* other)
{
	switch (m_type)
	{
	case Sensor::graze:
		if (typeid(*other) == typeid(Projectile))
		{
			std::cout << "puntos";
			//Award puntos
			//m_user->
		}
		break;
	case Sensor::itemGrab:
		/*if (typeid(*other) == typeid(Item))
		{

		}*/
		break;
	default:
		break;
	}
}
