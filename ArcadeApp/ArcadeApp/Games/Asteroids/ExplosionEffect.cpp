#include "ExplosionEffect.h"
#include "App.h"

ExplosionEffect::ExplosionEffect()
{

}

void ExplosionEffect::Init()
{
	mSpriteSheet.Load("AsteroidsSprites");

	mSprite.Init(App::Singleton().GetBasePath() + "Assets/AsteroidsAnimations.txt", mSpriteSheet);
	mSprite.SetAnimation("explosion", false);

	mIsFinishedExploding = true;
}

void ExplosionEffect::Activate(Vec2D Pos)
{
	mSprite.SetPosition(Pos);
	mIsFinishedExploding = false;
}

void ExplosionEffect::Update(uint32_t dt)
{
	if(!mIsFinishedExploding)
	{
		mSprite.Update(dt);

		if (mSprite.IsFinishedPlayingAnimation()) mIsFinishedExploding = true;
	}
}

void ExplosionEffect::Draw(Screen& theScreen)
{
	if(!mIsFinishedExploding)
	{
		mSprite.Draw(theScreen);
	}
}

