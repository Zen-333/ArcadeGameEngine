#pragma once

#include "Vec2D.h"
#include "SpriteSheet.h"
#include "AnimatedSprite.h"

class Missile {

public:

	Missile();
	void Init();
	void Update(uint32_t dt);
	void Launch(Vec2D& LaunchPoint);
	void Draw(Screen& theScreen);

private:

	void Deactivate();
	void Activate();

	const float mSpeed = 5;
	bool mActive = false;

	Vec2D mVelocity = Vec2D::Zero;
	SpriteSheet mSpriteSheet;
	AnimatedSprite mSprite;
	AnimatedSprite mThruster;
	Vec2D mLaunchPoint;
};