
#include "Spaceship.h"
#include "App.h"
#include "AARectangle.h"

Spaceship::Spaceship(const Vec2D SpawnPoint): mSpawnPoint(SpawnPoint)
{
	
}

void Spaceship::Init()
{
	mSpriteSheet.Load("AsteroidsSprites");
	mSprite.Init(App::Singleton().GetBasePath() + "Assets/AsteroidsAnimations.txt", mSpriteSheet);
	mSprite.SetAnimation("ship", true);
	mSprite.SetPosition(mSpawnPoint);
}

void Spaceship::Draw(Screen& theScreen)
{

	mSprite.Draw(theScreen);
	theScreen.Draw(GetFacingDirection(), Color::Green());
	
}

void Spaceship::Update(uint32_t dt)
{
	float dtSeconds = MillisecondsToSeconds(dt);

	if(mCanMove) 
	{
		mSprite.MoveBy(mVelocity * dtSeconds);
		mVelocity = mVelocity * 0.98f;
	}
	else { mVelocity = Vec2D::Zero; }

	mSprite.SetRotation(mAngleDegrees * (3.14159f / 180.0f));

	mSprite.Update(dt);
	
}

void Spaceship::MoveForward()
{
	if (!mCanMove) return;

	float radians = mAngleDegrees * (3.14159f / 180.0f);

	Vec2D forward = Vec2D(sinf(radians), -cosf(radians));

	mVelocity = forward * mForwardSpeed;
}


void Spaceship::RotateLeft()
{
	mAngleDegrees -= mRotationSpeed;
	if (mAngleDegrees < 0) mAngleDegrees += 360.0f;
}

void Spaceship::RotateRight()
{
	mAngleDegrees += mRotationSpeed;
	if (mAngleDegrees < 0) mAngleDegrees -= 360.0f;
}

Vec2D& Spaceship::GetFacingDirection()
{
	float radians = mAngleDegrees * (3.14159f / 180.0f);

	Vec2D forward = Vec2D(sinf(radians), -cosf(radians)) + Vec2D(mSprite.GetBoundingBox().GetCenterPoint().GetX(), mSprite.GetBoundingBox().GetCenterPoint().GetY() - 15.0f);
	
	forward.Rotate(radians, mSprite.GetBoundingBox().GetCenterPoint());

	return forward;
}

void Spaceship::Shoot()
{
}
