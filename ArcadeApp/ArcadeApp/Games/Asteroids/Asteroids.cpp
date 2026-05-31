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
	ShootKeyAction.Key = GameController::LeftMouseButton();
	ShootKeyAction.action = [this](uint32_t dt, InputState state)
		{
			if (mGameState == A_PLAY)
			{
				if (GameController::IsPressed(state))
				{
					mSpaceShip.Shoot();
				}
			}
		};

	controller.AddInputActionForKey(ShootKeyAction);



	mSpaceShip.Init();
}

void Asteroids::Update(uint32_t dt)
{
	bool CanMove = true;

	if (IsGreaterThanOrEual(mLevelBoundary.GetAARectangle().GetTopLeftPoint().GetX(), mSpaceShip.GetFacingDirection().GetX()))
	{
		CanMove = false;

	}
	else if (IsGreaterThanOrEual(mSpaceShip.GetFacingDirection().GetX(), mLevelBoundary.GetAARectangle().GetBottomRightPoint().GetX()))
	{
		CanMove = false;
	}

	if (IsGreaterThanOrEual(mSpaceShip.GetFacingDirection().GetY(), mLevelBoundary.GetAARectangle().GetBottomRightPoint().GetY()))
	{
		CanMove = false;
	}
	if(IsGreaterThanOrEual(mLevelBoundary.GetAARectangle().GetTopLeftPoint().GetY(), mSpaceShip.GetFacingDirection().GetY()))
	{
		CanMove = false;
	}

	mSpaceShip.SetCanMove(CanMove);


	mSpaceShip.Update(dt);
}

void Asteroids::Draw(Screen& screen)
{
	mSpaceShip.Draw(screen);
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
	//mCurrentTetromino.InitLevelBoundary(levelBoundary);
}
