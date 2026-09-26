#include "Pacman/PacmanGame.h"
#include "App.h"
#include "PacmanGameUtils.h"

namespace {
	const std::string SCORE_STR = "Score ";
}

void PacmanGame::Init(GameController& controller)
{
	mPacmanSpriteSheet.Load("PacmanSprites");
	mPacman.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Pacman_animations.txt", Vec2D::Zero, PACMAN_MOVEMENT_SPEED, false);

	mLevel.Init(App::Singleton().GetBasePath() + "Assets/Pacman_level.txt",&mPacmanSpriteSheet, &mPacman);


	ResetGame();

	ButtonAction leftAction;
	leftAction.Key = GameController::LeftKey();
	leftAction.action = [this](uint32_t dt, InputState state)
	{
			HandleGameControllerState(dt, state, PACMAN_MOVEMENT_LEFT);
	};
	controller.AddInputActionForKey(leftAction);

	ButtonAction rightAction;
	rightAction.Key = GameController::RightKey();
	rightAction.action = [this](uint32_t dt, InputState state)
		{
			HandleGameControllerState(dt, state, PACMAN_MOVEMENT_RIGHT);
		};
	controller.AddInputActionForKey(rightAction);

	ButtonAction upAction;
	upAction.Key = GameController::UpKey();
	upAction.action = [this](uint32_t dt, InputState state)
		{
			HandleGameControllerState(dt, state, PACMAN_MOVEMENT_UP);
		};
	controller.AddInputActionForKey(upAction);

	ButtonAction downAction;
	downAction.Key = GameController::DownKey();
	downAction.action = [this](uint32_t dt, InputState state)
		{
			HandleGameControllerState(dt, state, PACMAN_MOVEMENT_DOWN);
		};
	controller.AddInputActionForKey(downAction);
}

void PacmanGame::Update(uint32_t dt)
{
	UpdatePacmanMovement();
	mPacman.Update(dt);

	mLevel.Update(dt);

	if(mLevel.IsLevelOver())
	{
		mLevel.IncreaseLevel();
	}
}

void PacmanGame::Draw(Screen& screen)
{
	mLevel.Draw(screen);
	mPacman.Draw(screen);

	{
		Vec2D levelOffset = mLevel.GetLayoutOffset();

		AARectangle highScoreRect = AARectangle(Vec2D(0, 4), App::Singleton().Width(), levelOffset.GetY());

		const auto& font = App::Singleton().GetFont();
		Vec2D textDrawPosition;

		std::string scoreStr = std::to_string(mPacman.Score());

		textDrawPosition = font.GetDrawPosition(SCORE_STR + scoreStr, highScoreRect, BFXA_CENTER, BFYA_CENTER);

		screen.Draw(font, SCORE_STR + scoreStr, textDrawPosition);
	}
}

const std::string& PacmanGame::GetName() const
{
	static std::string name = "!Pacman Game!";
	return name;
}

void PacmanGame::ResetGame()
{
	mPressedDirection = PACMAN_MOVEMENT_NONE;
	mPacman.ResetScore();
	mLevel.ResetToFirstLevel();
}



void PacmanGame::UpdatePacmanMovement()
{
	if(mPressedDirection != PACMAN_MOVEMENT_NONE)
	{
		if(!mLevel.WillCollide(mPacman.GetBoundingBox(), mPressedDirection))
		{
			mPacman.SetMovementDirection(mPressedDirection);
		}
	}
}

void PacmanGame::HandleGameControllerState(uint32_t dt, InputState state, PacmanMovement direction)
{
	if(GameController::IsPressed(state))
	{
		mPressedDirection = direction;
	}else if(GameController::IsReleased(state) && mPressedDirection == direction)
	{
		mPressedDirection = PACMAN_MOVEMENT_NONE;
	}
}

