#pragma once

#include "Game.h"
#include "Spaceship.h"

enum AstroidGameState
{
	A_PLAY,
	A_GAME_OVER
};

class Asteroids : public Game
{

public:

	Asteroids();

	virtual void Init(GameController& controller) override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& screen) override;
	virtual const std::string& GetName() const override;

private:

	void ResetGame();

	Spaceship mSpaceShip;
	AstroidGameState mGameState;

};