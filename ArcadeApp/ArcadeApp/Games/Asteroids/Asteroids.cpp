#include "Asteroids.h"
#include "App.h"

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

}
