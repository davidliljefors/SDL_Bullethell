#pragma once
#include <string>

#include "Vector2D.h"
#include <SDL_pixels.h>
#include <SDL_render.h>

struct SDL_Renderer;

namespace FG
{
	class Window;
	class Camera
	{
	public:
		Vector2D position;

		virtual bool Initialize(Window* window, unsigned int flags = 2);
		virtual void Shutdown();

		virtual void StartRenderFrame();
		virtual void EndRenderFrame();

		void Shake(float intensity, float duration);
		void Update(float deltaTime);

		void SetColor(const SDL_Color& color);
		SDL_Color GetColor();

		SDL_Renderer* GetInternalRenderer() const { return renderer; }

	protected:
		Window* ownerWindow = nullptr;
		SDL_Renderer* renderer = nullptr;

		bool ScreenShake() { return shakeTime > 0; }
		
		float shakeTime;
		float shakeIntensity;

		float shakePauseDuration = 0.0f;
		float shakePauseTime;
	};
}
