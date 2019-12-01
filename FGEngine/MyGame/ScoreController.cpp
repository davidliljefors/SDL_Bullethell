#include "ScoreController.h"

#include <math.h>
#include <iostream>

int ScoreController::Update()
{

	if (score > displayScore)
	{
		if (score - displayScore > 1111)
		{
			displayScore += 1111;
		}
		else if (score - displayScore > 111)
		{
			displayScore += 111;
		}
		else if(score - displayScore > 11)
		{
			displayScore += 11;
		}
		else
		{
			displayScore += 1;
		}
	}
	return displayScore;
}

int ScoreController::Score()
{
	return score;
}

int ScoreController::HiScore()
{
	return hiScore;
}

void ScoreController::AddScore(int amount)
{
	score += abs(amount) * multiplier;
}

void ScoreController::UpdateMultiplier(int amount)
{
	multiplier += amount;
}

void ScoreController::SetHiScore(int value)
{
	hiScore = value;
}

bool ScoreController::NewHiScore()
{
	if (score > hiScore) {
		SetHiScore(score);
		return true;
	}
	return false;
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
