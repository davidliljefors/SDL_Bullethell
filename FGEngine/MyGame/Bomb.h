#pragma once
#include <Entity.h>
#include <Camera.h>

class Bomb : public FG::Entity 
{

public:
	Bomb(FG::Camera* camera, FG::Vector2D position, FG::Sprite* sprite) : camera(camera)
	{
		Entity::position = position;
		AddCircleCollider(0);
		this->sprite = sprite;
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

			//sprite->size += FG::Vector2D::One * 20;// *deltaTime;
			timeleft -= deltaTime;
		}
	}

	bool Activate(FG::Vector2D pos, float time)
	{
		if (timeleft > 0 )
			return false;
  		position = pos;
		collisionLayer.set(4);
		collider->SetRadius(10.f);
		//sprite->size = FG::Vector2D::One * 20;
		timeleft = time;
		return true;
	}

	void Render(FG::Camera* const camera) override
	{
		collider->Draw(camera, 255, 150, 0);

		Entity::Render(camera);
	}


	void OnCollision(Entity* other) override
	{

	}
private:
	float timeleft = 0;
	FG::Camera* camera = nullptr;
};