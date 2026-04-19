#pragma once

#include <string>


class HighScoreFile
{

public:

	HighScoreFile(const std::string& fileName);

	void CreateFile(std::string fileName);
	void ReadFile();
	void NewScore(int newScore);

private:

	std::string mFileName;
};

struct Score
{
	int mScore = 0;
	inline int GetScore() const { return mScore; }
	inline void IncreaseScore(const int points) { mScore += points; }
	inline void ResetScore() { mScore = 0; }
	inline void SaveScore(HighScoreFile file) { file.NewScore(mScore); ResetScore(); }
};