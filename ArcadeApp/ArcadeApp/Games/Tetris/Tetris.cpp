#include "Tetris.h"
#include "AARectangle.h"
#include "App.h"
#include <random>

Tetris::Tetris(): SHAPE_START_POS(Vec2D(App::Singleton().Width() / 2, 50)), 
mCurrentTetrisShape({ GetRandomShape(), Vec2D(20,20)})
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
	if (mCurrentTetrisShape.IsMoveLeftRequested())
	{
		if (WouldCollideWithBoard(mCurrentTetrisShape.GetTetrisShape(), MoveDirection::TD_LEFT))
			mCurrentTetrisShape.CancelMoveLeft();
	}

	if (mCurrentTetrisShape.IsMoveRightRequested())
	{
		if (WouldCollideWithBoard(mCurrentTetrisShape.GetTetrisShape(), MoveDirection::TD_RIGHT))
			mCurrentTetrisShape.CancelMoveRight();
	}

	mCurrentTetrisShape.Update(dt); // Processes approved left/right/rotate, sets mFallTickReady

	// Veto gravity tick before it moves
	if (mCurrentTetrisShape.IsFallTickReady())
	{
		if (WouldCollideWithBoard(mCurrentTetrisShape.GetTetrisShape(), MoveDirection::TD_DOWN))
		{
			mCurrentTetrisShape.CancelFall();
			mCurrentTetrisShape.StopFalling();
		}
		else
		{
			mCurrentTetrisShape.ApplyFall();
		}
	}

	if (!mCurrentTetrisShape.GetCanMoveDown())
	{
		LockCurrentShape();
		mCurrentTetrisShape = TetrisShapes({ GetRandomShape(), Vec2D(20, 20) });
		mCurrentTetrisShape.InitLevelBoundary(mLevelBoundary.GetAARectangle());
	}
}

void Tetris::Draw(Screen& screen)
{

	screen.Draw(mLevelBoundary.GetAARectangle(), Color::White(), false);

	Vec2D origin = mLevelBoundary.GetAARectangle().GetTopLeftPoint();

	for (int row = 0; row < BOARD_ROWS; row++)
	{
		for (int col = 0; col < BOARD_COLS; col++)
		{
			if (mBoardOccupied[row][col])
			{
				AARectangle cell = {
					origin + Vec2D(col * CELL_SIZE, row * CELL_SIZE),
					CELL_SIZE, CELL_SIZE
				};
				screen.Draw(cell, Color::White(), true, mBoard[row][col]);
			}
		}
	}

	mCurrentTetrisShape.Draw(screen);

	//screen.Draw(mLevelBoundary.GetAARectangle(), Color::White(), false);

	//mCurrentTetrisShape.Draw(screen);

	//for(auto& shapes : mArrayTetrisShapes)
	//{
	//	shapes.Draw(screen);
	//}
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

bool Tetris::WorldToBoard(const Vec2D& pixelPos, int& outCol, int& outRow)
{

	Vec2D origin = mLevelBoundary.GetAARectangle().GetTopLeftPoint();

	outCol = static_cast<int>((pixelPos.GetX() - origin.GetX()) / CELL_SIZE);
	outRow = static_cast<int>((pixelPos.GetY() - origin.GetY()) / CELL_SIZE);

	// Guard: reject out-of-bounds
	return outCol >= 0 && outCol < BOARD_COLS &&
		outRow >= 0 && outRow < BOARD_ROWS;

}

void Tetris::LockCurrentShape()
{
	for (const auto& rect : mCurrentTetrisShape.GetTetrisShape())
	{
		int col, row;
		if (WorldToBoard(rect.GetTopLeftPoint(), col, row))
		{
			mBoardOccupied[row][col] = true;
			mBoard[row][col] = mCurrentTetrisShape.GetColor(); // add GetColor() to TetrisShapes
		}
	}
}

bool Tetris::WouldCollideWithBoard(const std::vector<AARectangle>& rects, MoveDirection dir)
{
	for (const auto& rect : rects)
	{
		int col, row;

		switch (dir)
		{
		case MoveDirection::TD_DOWN:
		{
			// Check bottom-left and bottom-right corners of the bottom edge
			Vec2D checkLeft = { rect.GetTopLeftPoint().GetX(),     rect.GetBottomRightPoint().GetY() + 1 };
			Vec2D checkRight = { rect.GetBottomRightPoint().GetX(), rect.GetBottomRightPoint().GetY() + 1 };

			if (WorldToBoard(checkLeft, col, row) && mBoardOccupied[row][col])  return true;
			if (WorldToBoard(checkRight, col, row) && mBoardOccupied[row][col]) return true;
			break;
		}
		case MoveDirection::TD_LEFT:
		{
			// Check top-left and bottom-left corners of the left edge
			Vec2D checkTop = { rect.GetTopLeftPoint().GetX() - 1, rect.GetTopLeftPoint().GetY() };
			Vec2D checkBottom = { rect.GetTopLeftPoint().GetX() - 1, rect.GetBottomRightPoint().GetY() };

			if (WorldToBoard(checkTop, col, row) && mBoardOccupied[row][col])    return true;
			if (WorldToBoard(checkBottom, col, row) && mBoardOccupied[row][col]) return true;
			break;
		}
		case MoveDirection::TD_RIGHT:
		{
			// Check top-right and bottom-right corners of the right edge
			Vec2D checkTop = { rect.GetBottomRightPoint().GetX() + 1, rect.GetTopLeftPoint().GetY() };
			Vec2D checkBottom = { rect.GetBottomRightPoint().GetX() + 1, rect.GetBottomRightPoint().GetY() };

			if (WorldToBoard(checkTop, col, row) && mBoardOccupied[row][col])    return true;
			if (WorldToBoard(checkBottom, col, row) && mBoardOccupied[row][col]) return true;
			break;
		}
		}
	}
	return false;
}
