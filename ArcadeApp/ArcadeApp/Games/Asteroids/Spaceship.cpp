
#include "Spaceship.h"
#include "App.h"
#include "AARectangle.h"
#include "Missile.h"
#include <array>

Spaceship::Spaceship(const Vec2D SpawnPoint): mSpawnPoint(SpawnPoint)
{
	
}

void Spaceship::Init()
{
	mSpriteSheet.Load("AsteroidsSprites");

	mSprite.Init(App::Singleton().GetBasePath() + "Assets/AsteroidsAnimations.txt", mSpriteSheet);
	mSprite.SetAnimation("ship", true);
	mSprite.SetPosition(mSpawnPoint);

	mThruster.Init(App::Singleton().GetBasePath() + "Assets/AsteroidsAnimations.txt", mSpriteSheet);
	mThruster.SetAnimation("thrusters", true);
}

void Spaceship::Draw(Screen& theScreen)
{

	mSprite.Draw(theScreen);
	theScreen.Draw(GetFacingDirection(), Color::Green());
	theScreen.Draw(GetBackDirection(), Color::Red());
	

	if(mVelocity.Mag() > 1.0f)
	{
		float radians = mAngleDegrees * (3.14159f / 180.0f);

		Vec2D shipsPos = mSprite.Position();
		Vec2D shipCenter = mSprite.GetBoundingBox().GetCenterPoint();

		Vec2D BackDirection = GetBackDirection();
		Vec2D thrusterPos = Vec2D(BackDirection.GetX() - 4.5f , BackDirection.GetY() - 3.0f);

		mThruster.SetPosition(thrusterPos);
		mThruster.SetRotation(radians);
		mThruster.Draw(theScreen);
	}
	
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
	mThruster.Update(dt);
	
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

Vec2D Spaceship::GetFacingDirection()
{
	float radians = mAngleDegrees * (3.14159f / 180.0f);

	Vec2D forward = Vec2D(sinf(radians), -cosf(radians)) + Vec2D(mSprite.GetBoundingBox().GetCenterPoint().GetX(), mSprite.GetBoundingBox().GetCenterPoint().GetY() - 15.0f);
	
	forward.Rotate(radians, mSprite.GetBoundingBox().GetCenterPoint());

	return forward;
}

Vec2D Spaceship::GetBackDirection()
{
	float radians = mAngleDegrees * (3.14159f / 180.0f);

	Vec2D back = Vec2D(sinf(radians), -cosf(radians)) + Vec2D(mSprite.GetBoundingBox().GetCenterPoint().GetX(), mSprite.GetBoundingBox().GetCenterPoint().GetY() + 15.0f);

	back.Rotate(radians, mSprite.GetBoundingBox().GetCenterPoint());

	return back;
}

void Spaceship::Shoot(std::array<Missile, 3>& Missiles)
{
	for(Missile& m : Missiles)
	{
		if(!m.IsActive())
		{
			Vec2D FacingDir = GetFacingDirection();
			Vec2D LaunchPoint = Vec2D(FacingDir.GetX() - 3.3f, FacingDir.GetY() - 4);
			float radians = mAngleDegrees * (3.14159f / 180.0f);
			Vec2D Direction = Vec2D(sinf(radians), -cosf(radians));

			m.Launch(LaunchPoint, Direction, radians);
			return;
		}
	}
}
