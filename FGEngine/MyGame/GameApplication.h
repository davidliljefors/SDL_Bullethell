#pragma once

#include <Application.h>
#include <Time.h>

namespace FG
{
	class Window;
	class InputManager;
	class Camera;
	class EntityManager;
}

class GameApplication : public FG::Application
{
public:
	virtual bool Initialize() override;
	virtual void Run() override;
	virtual void Shutdown() override;

private:
	const float targetFramerate = 60;
	const float frameDelay = 1000 / targetFramerate;
	float frametime = 0;

	bool quit = false;
	FG::Time time;

	FG::Window* window = nullptr;
	FG::InputManager* inputManager = nullptr;
	FG::Camera* camera = nullptr;
	FG::EntityManager* entityManager = nullptr;
};

FG::Application* FG::CreateApplication()
{
	return new GameApplication();
}
