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
	Asteroid(EAsteroidSize Size, Vec2D SpawnPos);
	void Int();

	void Update(uint32_t dt);
	void Draw(Screen& theScreen);
	void Destroy();
	void Activate();

	inline void SetSpawnPos(const Vec2D SpawnPos) { mSpawnPos = SpawnPos; }
	inline const bool GetIsActive() const { return mIsActive; };

private:

	bool mIsActive = false;

	const float mMaxSpeed = 150;
	const float mMinSpeed = 50;

	const float mMaxSpin = 20;
	const float mMinSpin = 5;


	Vec2D mSpawnPos = Vec2D::Zero;

	SpriteSheet mSpriteSheet;
	AnimatedSprite mSprite;

	EAsteroidSize mAsteroidSize = AS_Small;
};