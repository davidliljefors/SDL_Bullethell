#pragma once

#include "GameState.h"
#include "Player.h"
#include "Obstacle.h"

namespace FG
{
	class EntityManager;
	class InputManager;
	class ResourceManager;
	class Text;
	class Sprite;
}
using namespace FG;

class StateManager
{
public:

	StateManager(FG::EntityManager* eManager, InputManager* iManager, ResourceManager* rManager, Camera* camera, Vector2D boundaries);
	~StateManager();

	void Update();
	void Render(Camera* const camera);

private:

	EntityManager* entityManager;
	Vector2D screenBoundaries;
	InputManager* inputManager;
	ResourceManager* resourceManager;

	Player* player;
	Obstacle* boss;

	Text* logo;
	Text* spacecPrompt;
	
	Text* playerLives;

};