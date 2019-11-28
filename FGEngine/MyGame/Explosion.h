#pragma once

#include "Entity.h"
#include <Camera.h>

class Explosion : public FG::Entity
{
public:

	Explosion(FG::Sprite* anim);

	void Update(float deltaTime) override;
	void Render(FG::Camera* const camera) override;

	void Explode(FG::Vector2D pos, float s);

private:

	float scale = 1;
};

