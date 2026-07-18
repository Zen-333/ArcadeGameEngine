#pragma once

#include "Game.h"
#include "Spaceship.h"
#include "LevelBoundary.h"
#include "Missile.h"
#include <array>
#include "Asteroid.h"
#include "App.h"
#include "HighScoreFile.h"

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
	void AddPoints(EAsteroidSize AsteroidSize);
	bool CanShipMove();
	bool CanMissileMove(const Missile& m);
	bool AreAllAsteroidsSpawned();
	Missile& DidMissileHitAsteroid(const Asteroid& a, bool& DidHit);
	bool DidAsteroidHitPlayer(const Asteroid& a);
	void ResetRandomSpawnTime();
	Vec2D GetRandomAsteroidPos();
	EAsteroidSize GetRandomAsteroidSize();
	void BreakAsteroid(Asteroid& a);

	void StartGame();

	Spaceship mSpaceShip;

	static constexpr size_t MAX_MISSILES = 3;
	static constexpr size_t MAX_START_ASTEROIDS = 2;
	static constexpr size_t MAX_TOTAL_ASTEROIDS = 32;
	SpriteSheet mAsteroidSpriteSheet;
	AnimatedSprite mLivesSprite;
	const Vec2D mLivesSpriteStartPos = {1, 5};
	const Vec2D mLivesSpriteIncrement = { 25, 0 };
	std::array<Missile, MAX_MISSILES> mMissiles;
	std::vector<Asteroid> mAsteroids;
	const float mAsteroidScreenPadding = 50.0f;
	const float mDistanceToScreenEntry = Vec2D(PLAYEBALE_AREA_WIDTH / 2.0f, PLAYEBALE_AREA_HEIGHT / 2.0f).Mag2() + mAsteroidScreenPadding + 160.0f;
	bool mSpawningAsteroids = true;

	const int mPlayerLives = 2;
	int mCurrentPlayerLive = mPlayerLives;
	int mPoints = 0;
	const int mSmallAsteroidPoint = 100;
	const int mMediumAsteroidPoint = 50;
	const int mLargeAsteroidPoint = 10;
	const unsigned int mScoreHeightPadding = 25;

	std::unordered_map<EAsteroidSize, int> mAsteroidPointsMap;

	float mAsteroidSpawnTime = 2.0f;
	float mTimePassed = 0.0f;
	float mMinSpawnTime = 2.0f;
	float mMaxSpawnTime = 5.0f;

	AstroidGameState mGameState;
	LevelBoundary mLevelBoundary;

	const uint32_t PLAYEBALE_AREA_WIDTH = 224;
	const uint32_t PLAYEBALE_AREA_HEIGHT = 280;

	const Vec2D mMiddleScreenPos = Vec2D(App::Singleton().Width() / 2.0f, App::Singleton().Height() / 2.0f);

	Score mScore;
	const std::string mFileName = "AsteroidsScoreTable.txt";
	HighScoreFile mScoreTable;

};