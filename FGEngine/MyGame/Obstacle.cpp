#include <typeinfo>
#include <Sprite.h>
#include <Camera.h>
#include <SDL_render.h>
#include "GameState.h"
#include <cassert>

#include "Obstacle.h"
#include "Projectile.h"

#include <iostream>
#include "Config.h"

Obstacle::Obstacle(FG::EntityManager* eManager, FG::ResourceManager* rManager, FG::Camera* camera) : entityManager(eManager), resourcecManager(rManager), camera(camera), entersScreen(false)
{
	collisionLayer.set(1);
	collisionLayer.set(0);
	projectilePool = new ProjectilePool(500,
		new Projectile(resourcecManager->GetResource<FG::Sprite>("bullet.png"), 5, false, FG::Vector2D::Down * 2000.f, 0, camera),
		entityManager);
}
void Obstacle::Initialize()
{

	Entity::AddSprite(sprites[static_cast<int>(phase)]);
}

void Obstacle::Update(float deltaTime)
{
	if (health <= 0)
	{
		EnterNextPhase();
	}
	isColliding = false;

	if (State::state == GAME_STATES::start) {
		if (!firstBattle && (phase != Phase::dead)) {
			float lerpSpeed = 2;
			if (position.y != startPosition.y || position.x != startPosition.x) {
				position = Lerp(position, startPosition, lerpSpeed * deltaTime);
			}
		}
		return;
	}

	if (entersScreen) {
		if (position.y != startPosition.y || position.x != startPosition.x) {
			position = Lerp(position, startPosition, 5 * deltaTime);
			if ((position.y <= startPosition.y + 5 && position.y >= startPosition.y - 5) &&
				(position.x <= startPosition.x + 5 && position.x >= startPosition.x - 5)) {
				entersScreen = false;
			}
		}
	}
	else {

	}
}


void Obstacle::Render(FG::Camera* const camera)
{
	Entity::Render(camera);
	DrawColliderCircle();
	//DrawBoundingBox();
}

void Obstacle::EnterNextPhase()
{
	assert(phase != Phase::dead);
	phase = static_cast<Phase>(static_cast<int>(phase) + 1);
	assert(sprites.size() > static_cast<int>(Phase::dead)); // Make sure we have enough sprites for all phasess
	Entity::AddSprite(sprites[static_cast<int>(phase)]);
	switch (phase)
	{
	case Phase::first:
		//should never happen
		break;
	case Phase::second:
		health = s_HealthValues[1];
		std::cout << "Enter second phase" << std::endl;
		break;
	case Phase::third:
		health = s_HealthValues[2];
		std::cout << "Enter Third Phase" << std::endl;
		break;
	case Phase::dead:
		// die
		health = s_HealthValues[3];
		OnDeath();
		std::cout << "boss died" << std::endl;
		break;
	default:
		break;
	}
}

SDL_Rect Obstacle::GetColliderRect()
{
	FG::Vector2D finalPosition = position - camera->position;
	return { static_cast<int>(finalPosition.x - (sprite->size.x / 2)), static_cast<int>(finalPosition.y - (sprite->size.y / 2)),
		static_cast<int>(sprite->size.x), static_cast<int>(sprite->size.y) };
}

void Obstacle::OnCollision(FG::Entity* other)
{
	if (typeid(*other) == typeid(Projectile)) {
		Projectile* p = static_cast<Projectile*>(other);
		health -= 1;
		//health -= p->damage;
		std::cout << health << std::endl;
	}
	isColliding = true;
}

void Obstacle::OnDeath()
{
	health = 999;
	entersScreen = false;
	PlaceOffscreenForEntrance();
}

void Obstacle::StartPosition(FG::Vector2D pos)
{
	position = startPosition = pos;
	PlaceOffscreenForEntrance();
}
void Obstacle::EnterScreen()
{
	entersScreen = true;
	SetUp();
	firstBattle = false;
}

void Obstacle::PlaceOffscreenForEntrance()
{
	position = startPosition - FG::Vector2D::Up * 250;
}

Phase Obstacle::CurrentPhase()
{
	return phase;
}

void Obstacle::SetUp()
{
	phase = Phase::first;
	health = s_HealthValues[0];
	Entity::AddSprite(sprites[static_cast<int>(phase)]);
}

bool Obstacle::AddSprite(FG::Sprite* spr)
{
	assert(spr);
	sprites.push_back(spr);
	return true;
}
	void Obstacle::DrawBoundingBox()
	{
#ifdef _DEBUG
		SDL_Color color = notCollidingColor;
		if (isColliding)
		{
			color = CollidingColor;
		}

		SDL_Rect finalRect = GetColliderRect();
		SDL_SetRenderDrawColor(camera->GetInternalRenderer(), color.r, color.g, color.b, color.a);

		SDL_RenderDrawRect(camera->GetInternalRenderer(), &finalRect);
		SDL_SetRenderDrawColor(camera->GetInternalRenderer(), 0, 0, 0, 255);
#endif _DEBUG
	}

	void Obstacle::DrawColliderCircle()
	{
#ifdef _DEBUG
		assert(collider);
		const int samples = 100;
		SDL_Color color = notCollidingColor;
		if (isColliding)
		{
			color = CollidingColor;
		}
		SDL_SetRenderDrawColor(camera->GetInternalRenderer(), color.r, color.g, color.b, color.a);
		FG::Vector2D positions[samples + 1];
		for (int i = 0; i < samples + 1; i++)
		{
			positions[i].x = sin(360.f / samples * i * 3.14159f / 180.f) * collider->GetRadius() + position.x;
			positions[i].y = cos(360.f / samples * i * 3.14159f / 180.f) * collider->GetRadius() + position.y;
		}
		for (int i = 0; i < samples; i++)
		{
			SDL_RenderDrawLine(camera->GetInternalRenderer(),
				(int)positions[i].x, (int)positions[i].y, (int)positions[i + 1].x, (int)positions[i + 1].y);
		}

		SDL_SetRenderDrawColor(camera->GetInternalRenderer(), 0, 0, 0, 255);
#endif _DEBUG
	}
