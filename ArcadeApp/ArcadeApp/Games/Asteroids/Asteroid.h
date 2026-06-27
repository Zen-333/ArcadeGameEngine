#pragma once

#include "SpriteSheet.h"
#include "AnimatedSprite.h"

enum EAsteroidSize {
	AS_Small,
	AS_Medium,
	AS_Large,
	AS_Count
};

class Asteroid{

public:

	Asteroid();
	Asteroid(EAsteroidSize Size, Vec2D SpawnPos, Vec2D MiddlePos);
	void Init(EAsteroidSize Size, Vec2D SpawnPos, Vec2D MiddlePos);

	void Update(uint32_t dt);
	void Draw(Screen& theScreen);
	void Destroy();
	void Activate();

	inline void SetSpawnPos(const Vec2D SpawnPos) { mSpawnPos = SpawnPos; }
	inline const bool GetIsActive() const { return mIsActive; };
	inline const bool GetHasEnteredTheScreen() const { return mHasEnteredTheScreen; }
	inline void SetHasEnteredTheScreen(const bool HasEnteredTheScreen) { mHasEnteredTheScreen = HasEnteredTheScreen; }
	inline const Vec2D GetPosition() { return mSprite.Position(); }

private:

	void ResetSpeedAndSpin();
	void SetForwardDirection();

	bool mIsActive = false;
	bool mHasEnteredTheScreen = false;

	const float mMaxSpeed = 30;
	const float mMinSpeed = 5;

	const float mMaxSpin = 3;
	const float mMinSpin = 1;

	float mSpeed = 0.0f;
	float mSpin = 0.0f;

	Vec2D mSpawnPos = Vec2D::Zero;
	Vec2D mForwardDir = Vec2D::Zero;
	Vec2D mMiddlePos = Vec2D::Zero;

	SpriteSheet mSpriteSheet;
	AnimatedSprite mSprite;

	EAsteroidSize mAsteroidSize = AS_Small;
};