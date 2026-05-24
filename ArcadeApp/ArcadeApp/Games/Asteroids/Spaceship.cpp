
#include "Spaceship.h"
#include "App.h"

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
	
}

void Spaceship::Update(uint32_t dt)
{

	mSprite.Update(dt);
	
}

