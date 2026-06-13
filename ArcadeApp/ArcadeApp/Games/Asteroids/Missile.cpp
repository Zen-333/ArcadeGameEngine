#include "Missile.h"
#include "App.h"

Missile::Missile()
{

}

void Missile::Init()
{
	mSpriteSheet.Load("AsteroidsSprites");

	mSprite.Init(App::Singleton().GetBasePath() + "Assets/AsteroidsAnimations.txt", mSpriteSheet);
	mSprite.SetAnimation("missile", true);
	//mSprite.SetPosition(mSpawnPoint);

	mIsActive = false;
}

void Missile::Update(uint32_t dt)
{
	if (!mIsActive) return;

	float dtSeconds = MillisecondsToSeconds(dt);

	mSprite.MoveBy(mVelocity * dtSeconds);
	mSprite.Update(dt);
	
}

void Missile::Launch(const Vec2D& LaunchPoint, const Vec2D& ForwardDirection, const float Rotation)
{
	float NewRotation = Rotation;
	if (NewRotation < 0) NewRotation -= 360.0f;

	mVelocity = ForwardDirection * mSpeed;
	mSprite.SetPosition(LaunchPoint);
	mSprite.RotateBy(NewRotation);
	mRotation = NewRotation;
	Activate();

}

void Missile::Draw(Screen& theScreen)
{
	if(mIsActive)
	{
		mSprite.Draw(theScreen);
	}
}

void Missile::Deactivate()
{
	mIsActive = false;
	mVelocity = Vec2D::Zero;
	mSprite.RotateBy(-mRotation);
	mRotation = 0.0f;
}

void Missile::Activate()
{
	mIsActive = true;
}

