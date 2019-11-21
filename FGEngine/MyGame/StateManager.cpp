#include "StateManager.h"
#include <Text.h>
#include <Sprite.h>
#include <EntityManager.h>
#include <InputManager.h>
#include <ResourceManager.h>

#include "Camera.h"

GAME_STATES State::state = GAME_STATES::start;

StateManager::StateManager(FG::EntityManager* eManager, InputManager* iManager, ResourceManager* rManager, Camera* camera, Vector2D boundaries) :
	entityManager(eManager), inputManager(iManager), resourceManager(rManager), screenBoundaries(boundaries)
{
	logo = new Text();
	logo->SetText(camera->GetInternalRenderer(), "Help me irl", "radiospace.ttf", 128, { 250,250,250 });
	
	spacecPrompt = new Text();
	spacecPrompt->SetText(camera->GetInternalRenderer(), "Press SPACE to dead", "radiospace.ttf", 36, { 225,225,225 });

	playerLives = new Text();
	playerLives->SetText(camera->GetInternalRenderer(), "Lives: 999", "radiospace.ttf", 36, { 250,250,250 });

	player = new Player(entityManager, inputManager, camera, screenBoundaries,
		new Projectile(resourceManager->GetResource<FG::Sprite>("bullet_sheet.png"), 5.5f, true, FG::Vector2D::Down * 2000.f, 0, camera, screenBoundaries));
	player->AddSprite(resourceManager->GetResource<FG::Sprite>("Bullethellplayer.png"));
	player->StartPosition({ 500, 650 });
	player->EnterScreen();
	player->AddCircleCollider(player->sprite->size.x / 8.f);
	player->AddColliderSprite(resourceManager->GetResource<FG::Sprite>("playercollider.png"));
	entityManager->AddEntity(player);

	//Boss
	boss = new Obstacle(camera);
	boss->AddSprite(resourceManager->GetResource<FG::Sprite>("hippie.png"));
	boss->StartPosition({ 500, 100 });
	boss->EnterScreen();
	boss->AddCircleCollider(64 / 2);
	entityManager->AddEntity(boss);
}

StateManager::~StateManager()
{

}

void StateManager::Update()
{
	switch (State::state)
	{
	case start:
		if (inputManager->IsKeyDown(SDL_SCANCODE_SPACE))
			State::state = game;
		break;
	case game:

		break;
	default:
		break;
	}
}

void StateManager::Render(Camera* const camera)
{
	switch (State::state)
	{
	case start:
		logo->Render(camera, { screenBoundaries.x / 2, screenBoundaries.y / 2 });
		spacecPrompt->Render(camera, { screenBoundaries.x / 2, screenBoundaries.y *.75f });
		break;
	case game:
		playerLives->Render(camera, { screenBoundaries.x * .125f, screenBoundaries.y * 0.05f});
		break;
	default:
		break;
	}
			
}
