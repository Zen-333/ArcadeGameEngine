#include "Asteroids.h"
#include "AARectangle.h"
#include "SpriteSheet.h"
#include <random>

Asteroids::Asteroids(): mSpaceShip(Vec2D(App::Singleton().Width() / 2.0f, App::Singleton().Height() / 2.0f))
{

}

void Asteroids::Init(GameController& controller)
{

	controller.ClearAll();

	mGameState = A_PLAY;

	ResetGame();

	mAsteroidPointsMap.insert(std::pair<EAsteroidSize, int>(EAsteroidSize::AS_Small, mSmallAsteroidPoint));
	mAsteroidPointsMap.insert(std::pair<EAsteroidSize, int>(EAsteroidSize::AS_Medium, mMediumAsteroidPoint));
	mAsteroidPointsMap.insert(std::pair<EAsteroidSize, int>(EAsteroidSize::AS_Large, mLargeAsteroidPoint));

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
			}else
			{
				StartGame();
			}
		};

	controller.AddInputActionForKey(ShootKeyAction);

	mSpaceShip.Init();

	mAsteroidSpriteSheet.Load("AsteroidsSprites");   
	mAsteroids.reserve(MAX_TOTAL_ASTEROIDS);

	mLivesSprite.Init(App::Singleton().GetBasePath() + "Assets/AsteroidsAnimations.txt", mAsteroidSpriteSheet);
	mLivesSprite.SetAnimation("ship", true);
	mLivesSprite.SetPosition(mLivesSpriteStartPos);

	for (Missile& m : mMissiles)
	{
		m.Init();
	}

	for (int i = 0; i < MAX_START_ASTEROIDS; i++)
	{
		Asteroid newAsteroid;
		newAsteroid.Init(GetRandomAsteroidSize(), GetRandomAsteroidPos(), mMiddleScreenPos, mAsteroidSpriteSheet);
		mAsteroids.push_back(newAsteroid);
	}

	ResetRandomSpawnTime();

}

void Asteroids::Update(uint32_t dt)
{
	mSpaceShip.Update(dt);
	if (mGameState == A_GAME_OVER) return;
	mSpaceShip.SetCanMove(CanShipMove());
	float dtSeconds = MillisecondsToSeconds(dt);

	if(mSpawningAsteroids)
	{
		mTimePassed += dtSeconds;
	}

	for (Missile& m : mMissiles)
	{
		m.Update(dt);
		
		if (m.IsActive() && !CanMissileMove(m)) m.Deactivate();
	}

	for (Asteroid& a : mAsteroids)
	{
		if (a.GetIsActive()) 
		{
			if(DidAsteroidHitPlayer(a))
			{
				mGameState = A_GAME_OVER;
				mSpaceShip.GotHit();
				ResetGame();
				return;
			}

			bool DidHit = false;
			if(a.GetPosition().Distance(mMiddleScreenPos) > mDistanceToScreenEntry)
			{
				a.Destroy();
				if (!a.GetCanRespawn()) return;
				mSpawningAsteroids = true;
				a.Respawn(GetRandomAsteroidSize(), GetRandomAsteroidPos(), mMiddleScreenPos);
				ResetRandomSpawnTime();
				return;
			}else
			{
				Missile& m = DidMissileHitAsteroid(a, DidHit);
				if(DidHit)
				{
					AddPoints(a.GetAsteroidSize());
					if(a.GetAsteroidSize() != AS_Small)
					{
						BreakAsteroid(a);
						m.Deactivate();
						a.Destroy();
						if (!a.GetCanRespawn()) return;
						mSpawningAsteroids = true;
						a.Respawn(GetRandomAsteroidSize(), GetRandomAsteroidPos(), mMiddleScreenPos);
						
					}else
					{
						a.Destroy();
						m.Deactivate();
						if (!a.GetCanRespawn()) return;
						mSpawningAsteroids = true;
						a.Respawn(GetRandomAsteroidSize(), GetRandomAsteroidPos(), mMiddleScreenPos);
						ResetRandomSpawnTime();
						return;
					}
				}
			}

			a.Update(dt);
		}else
		{
			if(a.GetCanRespawn() && mTimePassed > mAsteroidSpawnTime)
			{
				a.Activate();
				mTimePassed = 0.0f;
				ResetRandomSpawnTime();
			}
			if(AreAllAsteroidsSpawned() && mSpawningAsteroids)
			{
				mTimePassed = 0.0f;
				mSpawningAsteroids = false;
				return;
			}
		}

	}
}

void Asteroids::Draw(Screen& screen)
{

	mSpaceShip.Draw(screen);
	mLivesSprite.SetPosition(mLivesSpriteStartPos);

	for(int i = 0; i < mCurrentPlayerLive; i++)
	{
		mLivesSprite.Draw(screen);
		mLivesSprite.MoveBy(mLivesSpriteIncrement);
	}

	if (mGameState == A_GAME_OVER) return;

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

	for (Missile& m : mMissiles)
	{
		if(m.IsActive())
		{
			m.Deactivate();
		}
	}

	for(Asteroid& a : mAsteroids)
	{
		if(a.GetIsActive())
		{
			if(a.GetCanRespawn())
			{
				a.Respawn(GetRandomAsteroidSize(), GetRandomAsteroidPos(), mMiddleScreenPos);
			}
			a.Destroy();
		}
	}

	mSpawningAsteroids = true;

}

void Asteroids::AddPoints(EAsteroidSize AsteroidSize)
{
	for(auto& PointMap : mAsteroidPointsMap)
	{
		if(AsteroidSize == PointMap.first)
		{
			mPoints += PointMap.second;
			return;
		}
	}

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

bool Asteroids::AreAllAsteroidsSpawned()
{
	for (Asteroid& a : mAsteroids)
	{
		if(!a.GetIsActive() && a.GetCanRespawn())
		{
			return false;
		}
	}

	return true;

}

Missile& Asteroids::DidMissileHitAsteroid(const Asteroid& a, bool& DidHit)
{
	for (Missile& m : mMissiles)
	{
		if (!m.IsActive()) { DidHit = false; continue; }

		bool separated =
			m.GetSpriteBox().GetBottomRightPoint().GetX() < a.GetSpriteBox().GetTopLeftPoint().GetX() ||
			m.GetSpriteBox().GetTopLeftPoint().GetX() > a.GetSpriteBox().GetBottomRightPoint().GetX() ||
			m.GetSpriteBox().GetBottomRightPoint().GetY() < a.GetSpriteBox().GetTopLeftPoint().GetY() ||
			m.GetSpriteBox().GetTopLeftPoint().GetY() > a.GetSpriteBox().GetBottomRightPoint().GetY();

		if (!separated)
		{
			DidHit = true;
			return m;
		}
	}
	DidHit = false;
	return mMissiles[0];
}

bool Asteroids::DidAsteroidHitPlayer(const Asteroid& a)
{
	bool separated =
		mSpaceShip.GetSpriteBox().GetBottomRightPoint().GetX() < a.GetSpriteBox().GetTopLeftPoint().GetX() ||
		mSpaceShip.GetSpriteBox().GetTopLeftPoint().GetX() > a.GetSpriteBox().GetBottomRightPoint().GetX() ||
		mSpaceShip.GetSpriteBox().GetBottomRightPoint().GetY() < a.GetSpriteBox().GetTopLeftPoint().GetY() ||
		mSpaceShip.GetSpriteBox().GetTopLeftPoint().GetY() > a.GetSpriteBox().GetBottomRightPoint().GetY();

	return !separated;
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

void Asteroids::BreakAsteroid(Asteroid& a)
{
	Vec2D ParentPos = a.GetPosition();
	EAsteroidSize newSize = static_cast<EAsteroidSize>(a.GetAsteroidSize() - 1);
	Vec2D offsets[2] = { Vec2D(10.0f, 5.0f), Vec2D(-10.0f, 5.0f) };

	for(int i = 0; i < 2; i++)
	{
		Asteroid NewAsteroid;
		NewAsteroid.SetCanRespawn(false);
		NewAsteroid.Init(newSize, ParentPos + offsets[i], mMiddleScreenPos, mAsteroidSpriteSheet);
		NewAsteroid.Activate();
		mAsteroids.push_back(NewAsteroid);
	}

}

void Asteroids::StartGame()
{
	mCurrentPlayerLive--;
	if (mCurrentPlayerLive < 0)
	{
		mCurrentPlayerLive = mPlayerLives;
		mPoints = 0;
	}

	mSpaceShip.Reset();
	mGameState = A_PLAY;
}
