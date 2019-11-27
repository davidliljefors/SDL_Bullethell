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
	collider->Draw(camera, 0, 255, 255);
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
