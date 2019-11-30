#pragma once
#include <Entity.h>


class Obstacle;

namespace FG
{
	class Text;
}

class Healthbar : public FG::Entity {
public:
	Healthbar(FG::Vector2D position, FG::Vector2D size, Obstacle* boss);
	~Healthbar();
	void Update() {};
	void Render(FG::Camera* camera);
	void SetActive(bool state);
	int healthLastRender = 0;

private:
	FG::Vector2D position;
	FG::Vector2D size;
	Obstacle* boss = nullptr;
	FG::Text* textBarsLeft = nullptr;
	bool isActive = false;
};