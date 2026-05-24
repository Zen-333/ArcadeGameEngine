#pragma once

#include "Screen.h"
#include "SpriteSheet.h"
#include "AnimatedSprite.h"

class Spaceship
{

public:

	Spaceship();

	void Init();
	void Draw(Screen& theScreen);
	void Update(uint32_t dt);

private:

	const float mForwardSpeed = 5;
	const float mRotationSpeed = 5;

	SpriteSheet mSpriteSheet;
	AnimatedSprite mSprite;

};