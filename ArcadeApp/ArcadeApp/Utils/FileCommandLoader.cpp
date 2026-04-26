
#include "FileCommandLoader.h"
#include <fstream>
#include <iostream>
#include "Color.h"
#include "Vec2D.h"

void FileCommandLoader::AddCommand(const Command& command)
{
	mCommands.push_back(command);
}

bool FileCommandLoader::LoadFile(const std::string& filePath)
{
	std::ifstream inFile;

	inFile.open(filePath);

	std::string line = "";

	if (!inFile.is_open())
	{
		std::cout << "Could not open the file: " << filePath << std::endl;
		return false;
	}

	while (!inFile.eof()) // end of file
	{
		std::getline(inFile, line); // reads the next line from the file into line

		// Special static member constant value representing "no position" or "not found"
		size_t commandPos = std::string::npos;

		if ((commandPos = line.find(":")) != std::string::npos) // we assign commandPos and check it all in the if statement returns an index
		{
			// starting from commandPos, find the first space character
			size_t dilimitPos = line.find_first_of(" ", commandPos);

			// looking for where the line ends and the data begins
			if (dilimitPos == std::string::npos)
			{
				//If there's no space (e.g. just :level with nothing after), dilimitPos is set to the length of the string (pointing just past the end)
				dilimitPos = line.length();
			}
			else
			{
				// If there is a space, it backs up one: dilimitPos -= 1, so it now points to the last character of the keyword itself (the r in fillcolor)
				dilimitPos -= 1;
			}

			// substr(startPos, length) gets the command string example ":fillcolor"
			std::string commandStr = line.substr(commandPos + 1, dilimitPos);
			dilimitPos += 1;

			for (size_t commandIndex = 0; commandIndex < mCommands.size(); ++commandIndex)
			{
				if (commandStr == mCommands[commandIndex].command)
				{
					if (mCommands[commandIndex].commandType == COMMAND_ONE_LINE)
					{
						ParseFuncParams params;
						params.dilimitPos = dilimitPos;
						params.lineNum = 0;
						params.line = line;
						mCommands[commandIndex].parseFunc(params);
					}
					else
					{
						std::string numLines = line.substr(dilimitPos + 1); //  grabs the number after the keyword (e.g. "14")
						int totalLines = std::stoi(numLines); // converts std::string to int (stoi = string to int)
						int lineNum = 0;

						while (lineNum < totalLines)
						{
							std::getline(inFile, line);

							if (line.empty())
							{
								continue;
							}
							else
							{
								ParseFuncParams params;
								params.dilimitPos = dilimitPos;
								params.lineNum = lineNum;
								params.line = line;
								mCommands[commandIndex].parseFunc(params);
								++lineNum;
							}
						}
					}
				}
			}

		}
	}
	return true;
}

Color FileCommandLoader::ReadColor(const ParseFuncParams& params)
{
	size_t nextSpacePos = params.line.find_first_of(" ", params.dilimitPos + 1);
	int r = std::stoi(params.line.substr(params.dilimitPos, (nextSpacePos - params.dilimitPos)));

	size_t lastSpacePos = nextSpacePos;
	nextSpacePos = params.line.find_first_of(" ", lastSpacePos + 1);
	int g = std::stoi(params.line.substr(lastSpacePos + 1, nextSpacePos - lastSpacePos));

	lastSpacePos = nextSpacePos;
	nextSpacePos = params.line.find_first_of(" ", lastSpacePos + 1);
	int b = std::stoi(params.line.substr(lastSpacePos + 1, nextSpacePos - lastSpacePos));
	int a = std::stoi(params.line.substr(nextSpacePos + 1));

	return Color(r, g, b, a);
}

Vec2D FileCommandLoader::ReadSize(const ParseFuncParams& params)
{
	size_t nextSpacePos = params.line.find_first_of(" ", params.dilimitPos + 1);
	int width = std::stoi(params.line.substr(params.dilimitPos, nextSpacePos - params.dilimitPos));
	size_t lastSpacePos = nextSpacePos;
	nextSpacePos = params.line.find_first_of(" ", lastSpacePos + 1);
	int height = std::stoi(params.line.substr(lastSpacePos + 1, nextSpacePos - lastSpacePos));

	return Vec2D(width, height);
}

int FileCommandLoader::ReadInt(const ParseFuncParams& params)
{
	std::string intStr = params.line.substr(params.dilimitPos + 1);
	return std::stoi(intStr);
}

std::string FileCommandLoader::ReadString(const ParseFuncParams& params)
{
	return params.line.substr(params.dilimitPos + 1);
}

char FileCommandLoader::ReadChar(const ParseFuncParams& params)
{
	// [0] so it only takes the first character
	return params.line.substr(params.dilimitPos + 1)[0];
}

