#pragma once

#include "Screen.h"
#include "SpriteSheet.h"
#include "AnimatedSprite.h"

class ExplosionEffect {


public:

	ExplosionEffect();

	void Init();
	void Update(uint32_t dt);
	void Draw(Screen& theScreen);
	void Activate(Vec2D Pos);

private:

	bool mIsFinishedExploding = true;

	SpriteSheet mSpriteSheet;
	AnimatedSprite mSprite;

};