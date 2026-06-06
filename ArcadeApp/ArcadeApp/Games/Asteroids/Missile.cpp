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

	mThruster.Init(App::Singleton().GetBasePath() + "Assets/AsteroidsAnimations.txt", mSpriteSheet);
	mThruster.SetAnimation("thrusters", true); 
}

void Missile::Update(uint32_t dt)
{
	float dtSeconds = MillisecondsToSeconds(dt);

	if(mIsActive)
	{
		//mSprite.MoveBy(mVelocity * dtSeconds);
		mSprite.Update(dt);
		mThruster.Update(dt);
	}
}

void Missile::Launch(Vec2D LaunchPoint, Vec2D ForwardDirection)
{
	mSprite.SetPosition(LaunchPoint);
	mForwardDirection = ForwardDirection * mSpeed;
	Activate();

}

void Missile::Draw(Screen& theScreen)
{
	if(mIsActive)
	{
		mSprite.Draw(theScreen);
		mThruster.Draw(theScreen);
	}
}

void Missile::Deactivate()
{
	mIsActive = false;
}

void Missile::Activate()
{
	mIsActive = true;
}

Vec2D Missile::GetBackDirection()
{
	// TODO: Return the back direction of the missile to place the thrust;
	return Vec2D::Zero;
}
