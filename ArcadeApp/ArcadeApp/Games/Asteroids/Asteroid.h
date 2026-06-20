#pragma once

#include "SpriteSheet.h"
#include "AnimatedSprite.h"

enum EAsteroidSize {
	AS_Small,
	AS_Medium,
	AS_Large,
};

class Asteroid{

public:

	Asteroid();
	Asteroid(EAsteroidSize Size, Vec2D SpawnPos, Vec2D MiddlePos);
	void Int();

	void Update(uint32_t dt);
	void Draw(Screen& theScreen);
	void Destroy();
	void Activate();

	inline void SetSpawnPos(const Vec2D SpawnPos) { mSpawnPos = SpawnPos; }
	inline const bool GetIsActive() const { return mIsActive; };

private:

	void ResetSpeedAndSpin();
	void SetForwardDirection();

	bool mIsActive = false;

	const float mMaxSpeed = 30;
	const float mMinSpeed = 5;

	const float mMaxSpin = 5;
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