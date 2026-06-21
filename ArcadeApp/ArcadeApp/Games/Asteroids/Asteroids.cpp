#include "Asteroids.h"
#include "AARectangle.h"
#include <random>

Asteroids::Asteroids(): mSpaceShip(Vec2D(App::Singleton().Width() / 2.0f, App::Singleton().Height() / 2.0f))
{

}

void Asteroids::Init(GameController& controller)
{

	controller.ClearAll();

	mGameState = A_PLAY;

	ResetGame();

	ButtonAction ForwardKeyAction;
	ForwardKeyAction.Key = GameController::UpKey();
	ForwardKeyAction.action = [this](uint32_t dt, InputState state)
		{
			if (mGameState == A_PLAY)
			{
				if (GameController::IsPressed(state))
				{
					mSpaceShip.MoveForward();
				}
			}
		};

	controller.AddInputActionForKey(ForwardKeyAction);

	ButtonAction LeftRotationKeyAction;
	LeftRotationKeyAction.Key = GameController::LeftKey();
	LeftRotationKeyAction.action = [this](uint32_t dt, InputState state)
		{
			if (mGameState == A_PLAY)
			{
				if (GameController::IsPressed(state))
				{
					mSpaceShip.RotateLeft();
				}
			}
		};

	controller.AddInputActionForKey(LeftRotationKeyAction);

	ButtonAction RightRotationKeyAction;
	RightRotationKeyAction.Key = GameController::RightKey();
	RightRotationKeyAction.action = [this](uint32_t dt, InputState state)
		{
			if (mGameState == A_PLAY)
			{
				if (GameController::IsPressed(state))
				{
					mSpaceShip.RotateRight();
				}
			}
		};

	controller.AddInputActionForKey(RightRotationKeyAction);

	ButtonAction ShootKeyAction;
	ShootKeyAction.Key = GameController::ActionKey();
	ShootKeyAction.action = [this](uint32_t dt, InputState state)
		{
			if (mGameState == A_PLAY)
			{
				if (GameController::IsPressed(state))
				{
					mSpaceShip.Shoot(mMissiles);
				}
			}
		};

	controller.AddInputActionForKey(ShootKeyAction);

	mSpaceShip.Init();

	for (Missile& m : mMissiles)
	{
		m.Init();
	}

	for(Asteroid& a : mAsteroids)
	{
		a.Init(GetRandomAsteroidSize(), GetRandomAsteroidPos(), mMiddleScreenPos);
	}

	ResetRandomSpawnTime();

}

void Asteroids::Update(uint32_t dt)
{

	mSpaceShip.SetCanMove(CanShipMove());
	mSpaceShip.Update(dt);
	float dtSeconds = MillisecondsToSeconds(dt);

	if(mSpawningAsteroids)
	{
		mTimePassed += dtSeconds;
	}
	if (mTimePassed > 10.0f && mSpawningAsteroids) mSpawningAsteroids = false;

	for (Missile& m : mMissiles)
	{
		m.Update(dt);
		
		if (m.IsActive() && !CanMissileMove(m)) m.Deactivate();
			
	}

	for (Asteroid& a : mAsteroids)
	{
		if (a.GetIsActive()) 
		{
			a.Update(dt);
		}else
		{
			if(mTimePassed > mAsteroidSpawnTime)
			{
				a.Activate();
				std::cout << mTimePassed << std::endl;
				mTimePassed = 0.0f;
				ResetRandomSpawnTime();
			}
		}

	}
}

void Asteroids::Draw(Screen& screen)
{
	mSpaceShip.Draw(screen);

	for (Missile& m : mMissiles)
	{
		m.Draw(screen);
	}

	for (Asteroid& a : mAsteroids)
	{
		if (a.GetIsActive()) a.Draw(screen);
	}
}

const std::string& Asteroids::GetName() const
{
	static std::string name = "Asteroids <>";
	return name;
}

void Asteroids::ResetGame()
{
	AARectangle levelBoundary = { Vec2D::Zero, PLAYEBALE_AREA_WIDTH, PLAYEBALE_AREA_HEIGHT };
	mLevelBoundary = { levelBoundary };
}

bool Asteroids::CanShipMove()
{

	if (IsGreaterThanOrEual(mLevelBoundary.GetAARectangle().GetTopLeftPoint().GetX(), mSpaceShip.GetFacingDirection().GetX()))
	{
		return false;

	}
	else if (IsGreaterThanOrEual(mSpaceShip.GetFacingDirection().GetX(), mLevelBoundary.GetAARectangle().GetBottomRightPoint().GetX()))
	{
		return false;
	}

	if (IsGreaterThanOrEual(mSpaceShip.GetFacingDirection().GetY(), mLevelBoundary.GetAARectangle().GetBottomRightPoint().GetY()))
	{
		return false;
	}
	if (IsGreaterThanOrEual(mLevelBoundary.GetAARectangle().GetTopLeftPoint().GetY(), mSpaceShip.GetFacingDirection().GetY()))
	{
		return false;
	}

	return true;
}

bool Asteroids::CanMissileMove(const Missile& m)
{
	if (IsGreaterThanOrEual(mLevelBoundary.GetAARectangle().GetTopLeftPoint().GetX(), m.GetSpriteBox().GetTopLeftPoint().GetX()))
	{
		return false;

	}
	else if (IsGreaterThanOrEual(m.GetSpriteBox().GetBottomRightPoint().GetX(), mLevelBoundary.GetAARectangle().GetBottomRightPoint().GetX()))
	{
		return false;
	}

	if (IsGreaterThanOrEual(m.GetSpriteBox().GetBottomRightPoint().GetY(), mLevelBoundary.GetAARectangle().GetBottomRightPoint().GetY()))
	{
		return false;
	}
	if (IsGreaterThanOrEual(mLevelBoundary.GetAARectangle().GetTopLeftPoint().GetY(), m.GetSpriteBox().GetTopLeftPoint().GetY()))
	{
		return false;
	}

	return true;
}

void Asteroids::ResetRandomSpawnTime()
{
	static std::random_device r;
	static std::default_random_engine el(r());

	static std::uniform_real_distribution<float> uniform_dist(mMinSpawnTime, mMaxSpawnTime);

	mAsteroidSpawnTime = uniform_dist(el);
}

Vec2D Asteroids::GetRandomAsteroidPos()
{
	static std::random_device r;
	static std::default_random_engine el(r());

	static std::uniform_int_distribution<int> uniform_dist(0, 3);
	static std::uniform_int_distribution<int> uniform_dist_X(0, PLAYEBALE_AREA_WIDTH);
	static std::uniform_int_distribution<int> uniform_dist_Y(0, PLAYEBALE_AREA_HEIGHT);

	int Side = uniform_dist(el);

	float x = uniform_dist_X(el);
	float y = uniform_dist_Y(el);

	switch (Side)
	{
		case 0:
			y = -mAsteroidScreenPadding;
			break;
		case 1:
			y = mAsteroidScreenPadding + PLAYEBALE_AREA_HEIGHT;
			break;
		case 2:
			x = PLAYEBALE_AREA_WIDTH + mAsteroidScreenPadding;
			break;
		case 3:
			x = -mAsteroidScreenPadding;
			break;
	}

	return Vec2D(x, y);
}

EAsteroidSize Asteroids::GetRandomAsteroidSize()
{
	int Length = static_cast<int>(EAsteroidSize::AS_Count);

	std::random_device r;
	std::default_random_engine el(r());
	std::uniform_int_distribution<int> uniform_dist(0, Length - 1);

	int randomNumber = uniform_dist(el);

	return static_cast<EAsteroidSize>(randomNumber);
}
