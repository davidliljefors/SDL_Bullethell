#pragma once

#include <Application.h>
#include <Time.h>
#include "StateManager.h"
#include "AudioManager.h"
namespace FG
{
	class Window;
	class InputManager;
	class Camera;
	class EntityManager;
	class ResourceManager;
	class Text;
}

class GameApplication : public FG::Application
{
public:
	virtual bool Initialize() override;
	virtual void Run() override;
	virtual void Shutdown() override;

private:
	const float targetFramerate = 300;
	const float frameDelay = 1000 / targetFramerate;
	float frametime = 0;

	FG::Window* window = nullptr;
	FG::InputManager* inputManager = nullptr;
	FG::Camera* camera = nullptr;
	FG::EntityManager* entityManager = nullptr;
	FG::ResourceManager* resourceManager = nullptr;
	AudioManager* audioManager = nullptr;
	StateManager* stateManager = nullptr;

	FG::Time time;

	int GetScoreFromFile(const std::string& path);
	void WriteScoreToFile(int score, const std::string& path);

	FG::Text* testText = nullptr;
};

FG::Application* FG::CreateApplication()
{
	return new GameApplication();
}
