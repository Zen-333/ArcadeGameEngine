#pragma once

#include "Vec2D.h"
#include "SpriteSheet.h"
#include "AnimatedSprite.h"

class Missile {

public:

	Missile();
	void Init();
	void Update(uint32_t dt);
	void Launch(Vec2D LaunchPoint, Vec2D ForwardDirection);
	void Draw(Screen& theScreen);

private:

	void Deactivate();
	void Activate();
	
	Vec2D GetBackDirection();

	const float mSpeed = 5;
	bool mIsActive = false;

	Vec2D mVelocity = Vec2D::Zero;
	Vec2D mLaunchPoint;
	Vec2D mForwardDirection;
	SpriteSheet mSpriteSheet;
	AnimatedSprite mSprite;
	AnimatedSprite mThruster;
};