#pragma once
#include <Entity.h>
#include <Camera.h>

class Bomb : public FG::Entity 
{

public:
	Bomb(FG::Camera* camera, FG::Vector2D position) : camera(camera)
	{
		Entity::position = position;
		AddCircleCollider(0);
	}
	void Update(float deltaTime) override
	{
		if (timeleft < 0)
		{
			collisionLayer.reset(4);
			collider->SetRadius(0);
		}
		if (timeleft > 0)
		{
			collider->SetRadius(collider->GetRadius() + 25*(1+deltaTime));
			timeleft -= deltaTime;
		}
	}

	void Activate(FG::Vector2D pos, float time)
	{
		position = pos;
		collisionLayer.set(4);
		collider->SetRadius(10.f);
		timeleft = time;
	}

	void Render(FG::Camera* const camera) override
	{
#ifdef _DEBUG
		const int samples = 50;
		SDL_Color color = {255,255,0,0};

		SDL_SetRenderDrawColor(camera->GetInternalRenderer(), color.r, color.g, color.b, color.a);
		FG::Vector2D positions[samples + 1];
		for (int i = 0; i < samples + 1; i++)
		{
			positions[i].x = sin(360.f / samples * i * 3.14159f / 180.f) * collider->GetRadius() + position.x;
			positions[i].y = cos(360.f / samples * i * 3.14159f / 180.f) * collider->GetRadius() + position.y;
		}
		for (int i = 0; i < samples; i++)
		{
			SDL_RenderDrawLine(camera->GetInternalRenderer(),
				(int)positions[i].x, (int)positions[i].y, (int)positions[i + 1].x, (int)positions[i + 1].y);
		}

		SDL_SetRenderDrawColor(camera->GetInternalRenderer(), 0, 0, 0, 255);
#endif _DEBUG
	}


	void OnCollision(Entity* other) override
	{

	}
private:
	float timeleft = 0;
	FG::Camera* camera = nullptr;
};