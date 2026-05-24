
#include "Spaceship.h"
#include "App.h"

Spaceship::Spaceship()
{
	
}

void Spaceship::Init()
{
	mSpriteSheet.Load("AsteroidsSprites");
	mSprite.Init(App::Singleton().GetBasePath() + "Assets/AsteroidsAnimations.txt", mSpriteSheet);
	mSprite.SetAnimation("ship", false);
	bSpriteLoaded = true;
}

void Spaceship::Draw(Screen& theScreen)
{
	if(bSpriteLoaded)
	{
		mSprite.Draw(theScreen);
	}
}

void Spaceship::Update(uint32_t dt)
{
	if(bSpriteLoaded)
	{
		mSprite.Update(dt);
	}
}

