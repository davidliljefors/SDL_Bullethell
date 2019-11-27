#include "ScoreController.h"

#include <math.h>
#include <iostream>
int ScoreController::Update()
{

	if (score > displayScore)
	{
		if (score - displayScore >= 1000)
		{
			displayScore += 100;
		}
		else if(score - displayScore > 50)
		{
			displayScore += 10;
		}
		else
		{
			displayScore += 1;
		}
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
