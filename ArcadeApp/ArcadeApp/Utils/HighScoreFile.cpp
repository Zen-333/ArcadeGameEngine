#include "HighScoreFile.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


HighScoreFile::HighScoreFile(const std::string& fileName):mFileName(fileName)
{
	CreateFile(fileName);
}

void HighScoreFile::CreateFile(std::string fileName)
{
	ifstream inFile(fileName);
	
	if (inFile.good()) return;

	ofstream file(fileName);

	for(int i = 0; i < 10; i++)
	{
		file << "0\n";
	}

	file.close();
}

void HighScoreFile::ReadFile()
{
	std::string text;
	ifstream file(mFileName);

	while(getline (file, text))
	{
		std::cout << text;
	}

	file.close();
}

void HighScoreFile::NewScore(int newScore)
{
	ifstream fileIn(mFileName);
	std::vector<int> scores;
	string line;

	while(std::getline(fileIn, line))
	{
		if(!line.empty())
		{
			try
			{
				scores.push_back(std::stoi(line));
			}catch(...)
			{

			}
		}
	}
	fileIn.close();

	scores.push_back(newScore);

	std::sort(scores.begin(), scores.end(), std::greater<int>());

	if(scores.size() > 10)
	{
		scores.resize(10);
	}
	
	std::ofstream fileOut(mFileName, std::ios::trunc);

	for(int s : scores)
	{
		fileOut << s << "\n";
	}

	fileOut.close();

	

}
