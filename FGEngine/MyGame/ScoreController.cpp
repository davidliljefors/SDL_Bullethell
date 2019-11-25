#include "ScoreController.h"

#include <math.h>

long ScoreController::Update()
{
	if (score > displayScore)
	{
		displayScore = (score - displayScore) / 2;
	}
	return displayScore;
}

void ScoreController::AddScore(int amount)
{
	score += abs(amount) * multiplier;
}

void ScoreController::UpdateMultiplier(int amount)
{
	multiplier += amount;
}

void ScoreController::SetHiScore()
{
}

void ScoreController::ResetScore()
{
	score = 0;
	displayScore = 0;
}

void ScoreController::ResetMultiplier()
{
	multiplier = 1;
}
