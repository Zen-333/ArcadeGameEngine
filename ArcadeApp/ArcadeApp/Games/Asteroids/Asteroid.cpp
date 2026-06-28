#include "Asteroid.h"
#include "App.h"
#include <random>

Asteroid::Asteroid()
{
}

Asteroid::Asteroid(EAsteroidSize Size, Vec2D SpawnPos, Vec2D MiddlePos): mAsteroidSize(Size), mSpawnPos(SpawnPos), mMiddlePos(MiddlePos)
{
}

void Asteroid::Init(EAsteroidSize Size, Vec2D SpawnPos, Vec2D MiddlePos)
{
	mAsteroidSize = Size;
	mSpawnPos = SpawnPos;
	mMiddlePos = MiddlePos;

	mSpriteSheet.Load("AsteroidsSprites");

	mSprite.Init(App::Singleton().GetBasePath() + "Assets/AsteroidsAnimations.txt", mSpriteSheet);

	switch (mAsteroidSize)
	{
	case AS_Small:
		mSprite.SetAnimation("asteroid_small", true);
		break;
	case AS_Medium:
		mSprite.SetAnimation("asteroid_medium", true);
		break;
	case AS_Large:
		mSprite.SetAnimation("asteroid_large", true);
		break;
	default:
		mSprite.SetAnimation("asteroid_small", true);
		break;
	}

	mSprite.SetPosition(mSpawnPos);

	mIsActive = false;
	ResetSpeedAndSpin();
	SetForwardDirection();

}

void Asteroid::Update(uint32_t dt)
{
	if (!mIsActive) return;

	float dtSeconds = MillisecondsToSeconds(dt);

	mSprite.RotateBy(mSpin * dtSeconds);
	mSprite.MoveBy(mForwardDir * dtSeconds);


	mSprite.Update(dt);
}

void Asteroid::Draw(Screen& theScreen)
{
	if (!mIsActive) return;
	mSprite.Draw(theScreen);
}

void Asteroid::Destroy()
{
	mIsActive = false;

}

void Asteroid::Activate()
{
	mIsActive = true;

}

void Asteroid::ResetSpeedAndSpin()
{

	std::random_device r; 
	std::default_random_engine el(r()); 
	std::uniform_real_distribution<float> uniform_dist_spin(mMinSpin, mMaxSpin); 
	std::uniform_real_distribution<float> uniform_dist_speed(mMinSpeed, mMaxSpeed);

	mSpin = uniform_dist_spin(el);
	mSpeed = uniform_dist_speed(el);

}

void Asteroid::SetForwardDirection()
{
	Vec2D Direction = mMiddlePos - mSpawnPos;

	Direction.Normalize();

	mForwardDir = Direction * mSpeed;
}

void Asteroid::Respawn(EAsteroidSize size, Vec2D spawnPos, Vec2D middlePos)
{
	if(size != mAsteroidSize)
	{
		mAsteroidSize = size;
		switch (mAsteroidSize)
		{
		case AS_Small:
			mSprite.SetAnimation("asteroid_small", true);
			break;
		case AS_Medium:
			mSprite.SetAnimation("asteroid_medium", true);
			break;
		case AS_Large:
			mSprite.SetAnimation("asteroid_large", true);
			break;
		default:
			mSprite.SetAnimation("asteroid_small", true);
			break;
		}
	}
	mSpawnPos = spawnPos;
	mMiddlePos = middlePos;
	mSprite.SetPosition(mSpawnPos);
	
	ResetSpeedAndSpin();
	SetForwardDirection();
	mIsActive = false;
}
