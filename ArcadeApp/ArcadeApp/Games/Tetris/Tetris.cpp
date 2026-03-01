#include "Tetris.h"
#include "AARectangle.h"
#include "App.h"
#include <random>

Tetris::Tetris() : SHAPE_START_POS(Vec2D(App::Singleton().Width() / 2, 50)),
mCurrentTetrisShape({ GetRandomShape(), Vec2D(20,20) })
{

}

void Tetris::Init(GameController& controller)
{
	mGameState = TT_IN_PLAY;

	controller.ClearAll();

	ResetGame();

	ButtonAction leftKeyAction;
	leftKeyAction.Key = GameController::LeftKey();
	leftKeyAction.action = [this](uint32_t dt, InputState state)
		{
			if (mGameState == TT_IN_PLAY)
			{
				if (GameController::IsReleased(state))
				{
					mCurrentTetrisShape.SetMovementDirection(TetrisDirection::T_LEFT);
					mCurrentTetrisShape.RequestMoveLeft();
				}
				else
				{
					mCurrentTetrisShape.UnsetMovementDirection(TetrisDirection::T_LEFT);
					mCurrentTetrisShape.RequestMoveLeft();
				}
			}

		};

	controller.AddInputActionForKey(leftKeyAction);

	ButtonAction rightKeyAction;
	rightKeyAction.Key = GameController::RightKey();
	rightKeyAction.action = [this](uint32_t dt, InputState state)
		{
			if (mGameState == TT_IN_PLAY)
			{
				if (GameController::IsReleased(state))
				{
					mCurrentTetrisShape.SetMovementDirection(TetrisDirection::T_RIGHT);
					mCurrentTetrisShape.RequestMoveRight();
				}
				else
				{
					mCurrentTetrisShape.UnsetMovementDirection(TetrisDirection::T_RIGHT);
					mCurrentTetrisShape.RequestMoveRight();
				}

			}

		};

	controller.AddInputActionForKey(rightKeyAction);


	ButtonAction rotateAction;
	rotateAction.Key = GameController::ActionKey();
	rotateAction.action = [this](uint32_t dt, InputState state)
		{
			if (mGameState == TT_IN_PLAY)
			{
				if (GameController::IsPressed(state))
				{
					mCurrentTetrisShape.RequestRotate();
				}
			}

		};

	controller.AddInputActionForKey(rotateAction);
}

void Tetris::Update(uint32_t dt)
{
	mCurrentTetrisShape.Update(dt);

	if (!mCurrentTetrisShape.GetCanMoveDown())
	{
		// TODO: Spawn next shape
		TetrisShapes OldShape = mCurrentTetrisShape;
		mArrayTetrisShapes.push_back(OldShape);

		mCurrentTetrisShape = TetrisShapes({ GetRandomShape(), Vec2D(20,20) });

	}
}

void Tetris::Draw(Screen& screen)
{
	screen.Draw(mLevelBoundary.GetAARectangle(), Color::White(), false);

	mCurrentTetrisShape.Draw(screen);

	for (auto& shapes : mArrayTetrisShapes)
	{
		shapes.Draw(screen);
	}
}

const std::string& Tetris::GetName() const
{
	static std::string name = "Tetris <>";
	return name;
}

void Tetris::ResetGame()
{
	AARectangle levelBoundary = { Vec2D::Zero, PLAYEBALE_AREA_WIDTH, PLAYEBALE_AREA_HEIGHT };

	mLevelBoundary = { levelBoundary };
	mCurrentTetrisShape.InitLevelBoundary(levelBoundary);
}

bool Tetris::IsGameOver() const
{
	return false;
}

TetrisShapeType Tetris::GetRandomShape()
{
	int Length = static_cast<int>(TetrisShapeType::Count);

	std::random_device r;
	std::default_random_engine el(r());
	std::uniform_int_distribution<int> uniform_dist(0, Length);

	int randomNumber = uniform_dist(el);

	return static_cast<TetrisShapeType>(randomNumber);
}