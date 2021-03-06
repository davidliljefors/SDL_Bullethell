#include <SDL.h>
#include <SDL_ttf.h>
#include <Logger.h>
#include <Window.h>
#include <InputManager.h>
#include <Camera.h>
#include <EntityManager.h>
#include <ResourceManager.h>
#include <AudioManager.h>
#include <Sprite.h>
#include <Text.h>

#include "GameApplication.h"
#include "Player.h"
#include "Obstacle.h"
#include "Background.h"
#include "Config.h"
#include "Healthbar.h"

FG::Vector2D Config::screenBoundaries = { SCREENWIDTH , SCREENHEIGHT };

bool GameApplication::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		FG::Logger::Log(SDL_GetError(), FG::Logger::RemovePathFromFile(__FILE__), __LINE__);
		return false;
	}

	window = new FG::Window();
	if (!window->Initialize("My game", Config::SCREENWIDTH, Config::SCREENHEIGHT))
	{
		FG::Logger::Log(SDL_GetError(), FG::Logger::RemovePathFromFile(__FILE__), __LINE__);
		return false;
	}

	inputManager = new FG::InputManager();
	inputManager->Initialize();

	camera = new FG::Camera();
	if (!camera->Initialize(window, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))
	{
		return false;
	}

	if (TTF_Init() == -1) {
		printf("TTF Initialization Error: %s\n", TTF_GetError());
		return false;
	}

	resourceManager = new FG::ResourceManager();

	audioManager = new AudioManager(resourceManager);

	FG::Sprite* sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "Bullethellplayer.png");
	resourceManager->AddResource("Bullethellplayer.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "indicator.png");
	resourceManager->AddResource("indicator.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "BullethellPlayerRight.png");
	resourceManager->AddResource("BullethellPlayerRight.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "BullethellPlayerLeft.png");
	resourceManager->AddResource("BullethellPlayerLeft.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "BullethellBoss.png");
	resourceManager->AddResource("BullethellBoss.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "BullethellBoss2.png");
	resourceManager->AddResource("BullethellBoss2.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "BullethellBoss3.png");
	resourceManager->AddResource("BullethellBoss3.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "explo.png", 4, 4, 16);
	resourceManager->AddResource("explo.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "bullethellbg.png");
	resourceManager->AddResource("bullethellbg.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "bullethellbgSTARS.png");
	resourceManager->AddResource("bullethellbgSTARS.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "playercollider.png");
	resourceManager->AddResource("playercollider.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "bullet_sheet.png", 4, 2, 8);
	resourceManager->AddResource("bullet_sheet.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "bullet.png");
	resourceManager->AddResource("bullet.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "BullethellBulletPurple.png");
	resourceManager->AddResource("BullethellBulletPurple.png", sprite);
	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "BullethellBulletRed.png");
	resourceManager->AddResource("BullethellBulletRed.png", sprite);
	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "BullethellBulletCyan.png");
	resourceManager->AddResource("BullethellBulletCyan.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "BullethellBullet.png");
	resourceManager->AddResource("BullethellBullet.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "heart.png");
	resourceManager->AddResource("heart.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "heartcontainer.png");
	resourceManager->AddResource("heartcontainer.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "bomb.png");
	resourceManager->AddResource("bomb.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "bombcontainer.png");
	resourceManager->AddResource("bombcontainer.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "shockwave.png");
	resourceManager->AddResource("shockwave.png", sprite);

	entityManager = new FG::EntityManager();

	// Background layer 0 
	Background* bg1 = new Background(camera, 500);
	bg1->AddSprite(resourceManager->GetResource<FG::Sprite>("bullethellbg.png"));
	bg1->position.x = static_cast<float>(Config::SCREENWIDTH / 2);
	bg1->position.y = static_cast<float>(bg1->sprite->size.y * 0.5);
	entityManager->AddEntity(bg1);

	Background* bg2 = new Background(camera, 500);
	bg2->AddSprite(resourceManager->GetResource<FG::Sprite>("bullethellbg.png"));
	bg2->position.x = static_cast<float>(Config::SCREENWIDTH / 2);
	bg2->position.y = static_cast<float>(bg2->sprite->size.y * -0.5);
	entityManager->AddEntity(bg2);

	//Background layer 1, particles
	Background* bg3 = new Background(camera, 700);
	bg3->AddSprite(resourceManager->GetResource<FG::Sprite>("bullethellbgSTARS.png"));
	bg3->position.x = static_cast<float>(Config::SCREENWIDTH / 2);
	bg3->position.y = static_cast<float>(bg3->sprite->size.y * -0.5);
	entityManager->AddEntity(bg3);

	Background* bg4 = new Background(camera, 700);
	bg4->AddSprite(resourceManager->GetResource<FG::Sprite>("bullethellbgSTARS.png"));
	bg4->position.x = static_cast<float>(Config::SCREENWIDTH / 2);
	bg4->position.y = static_cast<float>(bg4->sprite->size.y * 0.5);
	entityManager->AddEntity(bg4);

	stateManager = new StateManager(entityManager, inputManager, audioManager, resourceManager, camera);

	return true;
}

void GameApplication::Run()
{
	float timescale = 1.f;
	bool quit = false;
	while (!quit)
	{
		time.StartFrame();
		inputManager->Update(quit);
		entityManager->Update(time.DeltaTime() * timescale);

		// quick n dirty slow motion
		timescale = 1.f;
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.user.code == 0xFFFF)
			{
				timescale = 0.5f;
			}
		}

		entityManager->DoCollisions();

		stateManager->Update();
		camera->Update(time.DeltaTime() * timescale);

		camera->StartRenderFrame();
		entityManager->Render(camera);
		stateManager->Render(camera);
		camera->EndRenderFrame();
		time.EndFrame();

		// Wait to achieve target framerate 
		if (time.DeltaTime() < frameDelay)
		{
			SDL_Delay(static_cast<Uint32>(frameDelay - time.DeltaTime()));
		}
		time.EndFrame();
	}
}

void GameApplication::Shutdown()
{
	delete testText;
	testText = NULL;

	if (stateManager)
	{
		delete stateManager;
		stateManager = nullptr;
	}

	if (entityManager)
	{
		entityManager->Shutdown();
		delete entityManager;
		entityManager = nullptr;
	}

	if (resourceManager)
	{
		resourceManager->Shutdown();
		delete resourceManager;
		resourceManager = nullptr;
	}

	if (camera)
	{
		camera->Shutdown();
		delete camera;
		camera = nullptr;
	}

	if (inputManager)
	{
		delete inputManager;
		inputManager = nullptr;
	}


	if (window)
	{
		window->Shutdown();
		delete window;
		window = nullptr;
	}

	if (audioManager)
	{
		delete audioManager;
		audioManager = nullptr;
	}
	TTF_Quit();
	SDL_Quit();
}