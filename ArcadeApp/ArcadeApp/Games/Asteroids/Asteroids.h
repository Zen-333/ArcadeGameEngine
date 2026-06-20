#pragma once

#include "Game.h"
#include "Spaceship.h"
#include "LevelBoundary.h"
#include "Missile.h"
#include <array>
#include "Asteroid.h"

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
	bool CanShipMove();
	bool CanMissileMove(const Missile& m);

	Spaceship mSpaceShip;
	Asteroid mAsteroid;

	static constexpr size_t MAX_MISSILES = 3;
	std::array<Missile, MAX_MISSILES> mMissiles;

	AstroidGameState mGameState;
	LevelBoundary mLevelBoundary;

	const uint32_t PLAYEBALE_AREA_WIDTH = 224;
	const uint32_t PLAYEBALE_AREA_HEIGHT = 280;

};