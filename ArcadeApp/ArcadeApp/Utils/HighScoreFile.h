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
