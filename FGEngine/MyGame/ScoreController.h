#pragma once
class ScoreController
{
public:
	ScoreController() {}

	long Update();

	void AddScore(int amount);
	void UpdateMultiplier(int amount);

	void SetHiScore();
	void NewHiScore();

private:
	void ResetScore();
	void ResetMultiplier();

	long score = 0;
	long displayScore = 0;
	long hiScore = 0;

	bool beatHiScore;

	int multiplier = 1;

	//int gradeLOLOLOLOL = 0;
};

