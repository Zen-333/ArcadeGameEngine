#include "Asteroid.h"
#include "App.h"

Asteroid::Asteroid()
{
}

Asteroid::Asteroid(EAsteroidSize Size, Vec2D SpawnPos): mAsteroidSize(Size), mSpawnPos(SpawnPos)
{
}

void Asteroid::Int()
{
	mSpriteSheet.Load("AsteroidsSprites");

	mSprite.Init(App::Singleton().GetBasePath() + "Assets/AsteroidsAnimations.txt", mSpriteSheet);

	switch (mAsteroidSize)
	{
	case AS_Small:
		mSprite.SetAnimation("missile", true);
		break;
	case AS_Medium:
		mSprite.SetAnimation("missile", true);
		break;
	case AS_Large:
		mSprite.SetAnimation("missile", true);
		break;
	default:
		mSprite.SetAnimation("missile", true);
		break;
	}

	mIsActive = false;
}

void Asteroid::Update(uint32_t dt)
{
}

void Asteroid::Draw(Screen& theScreen)
{
}

void Asteroid::Destroy()
{
	mIsActive = false;
}

void Asteroid::Activate()
{
	mIsActive = true;
}
