#pragma once

#include "Vec2D.h"
#include "SpriteSheet.h"
#include "AnimatedSprite.h"

class Missile {

public:

	Missile();
	void Init();
	void Update(uint32_t dt);
	void Launch(const Vec2D& LaunchPoint, const Vec2D& ForwardDirection, const float Rotation);
	void Draw(Screen& theScreen);

	inline bool IsActive() const { return mIsActive; }
	void Deactivate();

private:

	void Activate();

	const float mSpeed = 200.0f;
	bool mIsActive = false;

	Vec2D mVelocity = Vec2D::Zero;

	SpriteSheet mSpriteSheet;
	AnimatedSprite mSprite;
};