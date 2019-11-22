#include "StateManager.h"
#include <Text.h>
#include <Sprite.h>
#include <EntityManager.h>
#include <InputManager.h>
#include <ResourceManager.h>

#include "Camera.h"
#include "Config.h"

GAME_STATES State::state = GAME_STATES::start;

StateManager::StateManager(FG::EntityManager* eManager, InputManager* iManager, AudioManager* aManager, ResourceManager* rManager, Camera* camera) :
	entityManager(eManager), inputManager(iManager), audioManager(aManager), resourceManager(rManager), screenBoundaries(Config::screenBoundaries)
{
	// ENTITIES
	player = new Player(entityManager, inputManager, audioManager, camera,
		new Projectile(resourceManager->GetResource<FG::Sprite>("bullet_sheet.png"), 5.5f, true, FG::Vector2D::Down * 2000.f, 0, camera));
	player->AddSprite(resourceManager->GetResource<FG::Sprite>("Bullethellplayer.png"));
	player->StartPosition({ 500, 650 });
	player->AddCircleCollider(player->sprite->size.x / 8.f);
	player->AddColliderSprite(resourceManager->GetResource<FG::Sprite>("playercollider.png"));
	entityManager->AddEntity(player);

	//Boss
	boss = new Obstacle(entityManager, resourceManager, camera);
	boss->StartPosition({ 500, 100 });
	boss->AddCircleCollider(64 / 2);
	boss->AddSprite(resourceManager->GetResource<FG::Sprite>("hippie.png"));
	boss->AddSprite(resourceManager->GetResource<FG::Sprite>("hippie2.png"));
	boss->AddSprite(resourceManager->GetResource<FG::Sprite>("hippie3.png"));
	boss->AddSprite(resourceManager->GetResource<FG::Sprite>("hippie4.png"));
	boss->Initialize();
	entityManager->AddEntity(boss);

	//UI ELEMENTS
	logo = new Text();
	logo->SetText(camera->GetInternalRenderer(), "Help me irl", "radiospace.ttf", 128, { 250,250,250 });
	resourceManager->AddResource("Help me irl", logo);

	spacePrompt = new Text();
	spacePrompt->SetText(camera->GetInternalRenderer(), "Press SPACE to dead", "radiospace.ttf", 36, { 225,225,225 });
	resourceManager->AddResource("Press SPACE to dead", spacePrompt);

	hiScoreDisplay = new Text();
	hiScoreDisplay->SetText(camera->GetInternalRenderer(), "Press SPACE to dead", "radiospace.ttf", 36, { 225,225,225 });
	resourceManager->AddResource("Press SPACE to dead", hiScoreDisplay);

	playerLives = new Sprite * [player->maxLives];
	liveContainers = new Sprite * [player->maxLives];
	for (size_t i = 0; i < player->maxLives; i++)
	{
		playerLives[i] = resourceManager->GetResource<FG::Sprite>("heart.png");
		liveContainers[i] = resourceManager->GetResource<FG::Sprite>("heartcontainer.png");
	}
	for (size_t i = 0; i < player->maxLives; i++)
	{
	}

	currentScoreDisplay = new Text();
	currentScoreDisplay->SetText(camera->GetInternalRenderer(), "Press SPACE to dead", "radiospace.ttf", 36, { 225,225,225 });
	resourceManager->AddResource("Press SPACE to dead", currentScoreDisplay);

	currentHiScoreDisplay = new Text();
	currentHiScoreDisplay->SetText(camera->GetInternalRenderer(), "Press SPACE to dead", "radiospace.ttf", 36, { 225,225,225 });
	resourceManager->AddResource("Press SPACE to dead", currentHiScoreDisplay);

	firstBattle = true;
}

StateManager::~StateManager()
{

}

void StateManager::Update()
{
	switch (State::state)
	{
	case start:
		if (inputManager->IsKeyDown(SDL_SCANCODE_SPACE)) {
			State::state = game;
			player->OnStartBattle();
			boss->EnterScreen();
		}
		break;
	case game:
		if (boss->CurrentPhase() == Phase::dead) {
			//boss->Reset();
			player->OnVictory();
			State::state = start;
		}
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
		spacePrompt->Render(camera, { screenBoundaries.x / 2, screenBoundaries.y *.75f });
		break;
	case game:
		for (size_t i = 0; i < player->maxLives; i++)
		{
			if (i < player->CurrentLives())
				playerLives[i]->Render(camera, { screenBoundaries.x - (playerLives[i]->size.x / 2) * 2, 0 + (playerLives[i]->size.y / 2) * (2 * (i + 1)) });
			liveContainers[i]->Render(camera, { screenBoundaries.x - (playerLives[i]->size.x /2) * 2, 0 + (playerLives[i]->size.y /2 )*(2*(i+1)) });
		}
		break;
	default:
		break;
	}
			
}
