#include <iostream>

//#pragma region memestuff
//// Overloading new and delete to keep track of memory leakage
//static int alloc = 0;
//void* operator new(std::size_t size)
//{
//	alloc += 1;
//	std::cout << "Alloc : " << size << "bytes, count:" << alloc << std::endl;
//	return malloc(size);
//}
//void operator delete(void* p)
//{
//	alloc -= 1;;
//	std::cout << "Free, count:" << alloc << std::endl;
//	free(p);
//}
//#pragma endregion memestuff

#include <SDL.h>
#include <Logger.h>
#include <Window.h>
#include <InputManager.h>
#include <Camera.h>
#include <EntityManager.h>
#include <ResourceManager.h>
#include <Sprite.h>


#include "GameApplication.h"
#include "Player.h"
#include "Obstacle.h"
#include "Background.h"



bool GameApplication::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		FG::Logger::Log(SDL_GetError(), FG::Logger::RemovePathFromFile(__FILE__), __LINE__);
		return false;
	}

	window = new FG::Window();
	if (!window->Initialize("My game", SCREENWIDTH, SCREENHEIGHT))
	{
		FG::Logger::Log(SDL_GetError(), FG::Logger::RemovePathFromFile(__FILE__), __LINE__);
		return false;
	}

	inputManager = new FG::InputManager();
	inputManager->Initialize();

	camera = new FG::Camera();
	if (!camera->Initialize(window))
	{
		return false;
	}


	resourceManager = new FG::ResourceManager();
	FG::Sprite* sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "Bullethellplayer.png");
	resourceManager->AddResource("Bullethellplayer.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "hippie.png");
	resourceManager->AddResource("hippie.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "bullethellbg.png");
	resourceManager->AddResource("bullethellbg.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "bullethellbgSTARS.png");
	resourceManager->AddResource("bullethellbgSTARS.png", sprite);

	sprite = new FG::Sprite();
	sprite->LoadImage(camera->GetInternalRenderer(), "bullet_sheet.png", 4, 2, 8);
	resourceManager->AddResource("bullet_sheet.png", sprite);

	entityManager = new FG::EntityManager();

	// Background layer 0
	Background* bg1 = new Background(camera, 5);
	bg1->AddSprite(resourceManager->GetResource<FG::Sprite>("bullethellbg.png"));
	bg1->position.x = static_cast<float>(SCREENWIDTH / 2);
	bg1->position.y = static_cast<float>(bg1->sprite->size.y*0.5);
	entityManager->AddEntity(bg1);

	Background* bg2 = new Background(camera, 5);
	bg2->AddSprite(resourceManager->GetResource<FG::Sprite>("bullethellbg.png"));
	bg2->position.x = static_cast<float>(SCREENWIDTH / 2);
	bg2->position.y = static_cast<float>(bg2->sprite->size.y * -0.5);
	entityManager->AddEntity(bg2);

	//Background layer 1, particles
	Background* bg3 = new Background(camera, 7);
	bg3->AddSprite(resourceManager->GetResource<FG::Sprite>("bullethellbgSTARS.png"));
	bg3->position.x = static_cast<float>(SCREENWIDTH / 2);
	bg3->position.y = static_cast<float>(bg3->sprite->size.y * -0.5);
	entityManager->AddEntity(bg3);

	Background* bg4 = new Background(camera, 7);
	bg4->AddSprite(resourceManager->GetResource<FG::Sprite>("bullethellbgSTARS.png"));
	bg4->position.x = static_cast<float>(SCREENWIDTH / 2);
	bg4->position.y = static_cast<float>(bg4->sprite->size.y *0.5);
	entityManager->AddEntity(bg4);


	Player* player = new Player(entityManager, inputManager, camera, { (float)SCREENWIDTH, (float)SCREENHEIGHT },
		new Projectile(resourceManager->GetResource<FG::Sprite>("bullet_sheet.png"), 5.5f, true, FG::Vector2D::Down * 1000.f, camera, { (float)SCREENWIDTH, (float)SCREENHEIGHT }));
	player->AddSprite(resourceManager->GetResource<FG::Sprite>("Bullethellplayer.png"));
	player->StartPosition({ 500, 600 });
	player->AddCircleCollider(player->sprite->size.x / 7.5f);
	entityManager->AddEntity(player);
	/*player->projectilePrefab =
		new Projectile(resourceManager->GetResource<FG::Sprite>("bullet.png"), 0.5f, true, FG::Vector2D::Down*2000.f, camera, { (float)SCREENWIDTH, (float)SCREENHEIGHT });*/

	//Boss
	Obstacle* obstacle = new Obstacle(camera);
	obstacle->AddSprite(resourceManager->GetResource<FG::Sprite>("hippie.png"));
	obstacle->position.x = 500.f;
	obstacle->position.y = 100.f;
	obstacle->AddCircleCollider(64 / 2);
	entityManager->AddEntity(obstacle);



	return true;
}

void GameApplication::Run()
{
	bool quit = false;
	while (!quit)
	{
		time.StartFrame();
		inputManager->Update(quit);
		entityManager->Update(time.DeltaTime());
		entityManager->DoCollisions();
		camera->StartRenderFrame();
		entityManager->Render(camera);
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

	SDL_Quit();
}
