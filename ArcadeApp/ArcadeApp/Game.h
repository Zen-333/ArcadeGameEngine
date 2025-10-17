#pragma once

#include <string>
#include <stdint.h>

class GameController;
class Screen;

// interface

class Game
{
public:
	virtual ~Game(){}
	virtual void Init(GameController& controller) = 0;
	virtual void Update(uint32_t dt) = 0;
	virtual void Draw(Screen& screen) = 0;
	virtual std::string GetName() const = 0;
};