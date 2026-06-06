#pragma once

#include "Game.h"
#include "Spaceship.h"
#include "LevelBoundary.h"
#include "Missile.h"
#include "vector"

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

	std::vector<Missile> mMissiles;
	std::vector<Missile&> mActiveMissiles;
	std::vector<Missile&> mNotActiveMissiles;
	const size_t mMaxMissiles = 2;

	AstroidGameState mGameState;
	LevelBoundary mLevelBoundary;

	const uint32_t PLAYEBALE_AREA_WIDTH = 224;
	const uint32_t PLAYEBALE_AREA_HEIGHT = 280;

};