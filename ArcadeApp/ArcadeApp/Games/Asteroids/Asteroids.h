#pragma once

#include "Game.h"
#include "Spaceship.h"
#include "LevelBoundary.h"
#include "Missile.h"
#include <array>
#include "Asteroid.h"
#include "App.h"

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
	void ResetRandomSpawnTime();
	Vec2D GetRandomAsteroidPos();
	EAsteroidSize GetRandomAsteroidSize();

	Spaceship mSpaceShip;

	static constexpr size_t MAX_MISSILES = 3;
	static constexpr size_t MAX_ASTEROIDS = 6;
	std::array<Missile, MAX_MISSILES> mMissiles;
	std::array<Asteroid, MAX_ASTEROIDS> mAsteroids;
	const float mAsteroidScreenPadding = 50.0f;
	bool mSpawningAsteroids = true;

	float mAsteroidSpawnTime = 2.0f;
	float mTimePassed = 0.0f;
	float mMinSpawnTime = 2.0f;
	float mMaxSpawnTime = 5.0f;

	AstroidGameState mGameState;
	LevelBoundary mLevelBoundary;

	const uint32_t PLAYEBALE_AREA_WIDTH = 224;
	const uint32_t PLAYEBALE_AREA_HEIGHT = 280;

	const Vec2D mMiddleScreenPos = Vec2D(App::Singleton().Width() / 2.0f, App::Singleton().Height() / 2.0f);

};