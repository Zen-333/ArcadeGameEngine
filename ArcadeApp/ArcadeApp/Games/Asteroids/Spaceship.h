#pragma once

#include "Screen.h"
#include "SpriteSheet.h"
#include "AnimatedSprite.h"

class Spaceship
{

public:

	Spaceship(const Vec2D SpawnPoint);

	void Init();
	void Draw(Screen& theScreen);
	void Update(uint32_t dt);
	void MoveForward();
	void Shoot();
	void RotateLeft();
	void RotateRight();

	inline void SetCanMove(const bool CanMove) { mCanMove = CanMove; }

	Vec2D& GetFacingDirection();

private:

	Vec2D& GetBackDirection();

	bool mCanMove = true;

	const float mForwardSpeed = 100.0f;
	const float mRotationSpeed = 3.0f;

	float mAngleDegrees = 0.0f;
	Vec2D mVelocity = Vec2D::Zero;

	SpriteSheet mSpriteSheet;
	AnimatedSprite mSprite;
	AnimatedSprite mThruster;

	Vec2D mSpawnPoint;

};