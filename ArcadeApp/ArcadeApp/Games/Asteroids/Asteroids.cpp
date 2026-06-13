#include "Asteroids.h"
#include "App.h"
#include "AARectangle.h"

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

}

void Asteroids::Update(uint32_t dt)
{

	mSpaceShip.SetCanMove(CanShipMove());
	mSpaceShip.Update(dt);

	for (Missile& m : mMissiles)
	{
		if(m.IsActive())
		{
			if(CanMissileMove(m))
			{
				m.Update(dt);
			}
			else m.Deactivate();
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
