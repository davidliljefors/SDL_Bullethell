#pragma once

class ScoreController
{ 
public:
	ScoreController() {}

	int Update();

	void AddScore(int amount);
	void UpdateMultiplier(int amount);

	void SetHiScore();
	void NewHiScore();

private:
	void ResetScore();
	void ResetMultiplier();

	int score = 0;
	int displayScore = 0;
	int hiScore = 0;

	bool beatHiScore;

	int multiplier = 1;

	//int gradeLOLOLOLOL = 0;
};

