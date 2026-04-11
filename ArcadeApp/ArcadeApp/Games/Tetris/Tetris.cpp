#include "Tetris.h"
#include "AARectangle.h"
#include "App.h"
#include <random>
#include "Color.h"

Tetris::Tetris() : SHAPE_START_POS(Vec2D(App::Singleton().Width() / 2, 50)),
mCurrentTetromino({ GetRandomShape(), Vec2D(20,20) }), mNextShapeType(GetRandomShape()), mNextShapeWindow(65, 65, Vec2D(155, 5), mNextShapeType)
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
				if (GameController::IsPressed(state))
				{
					mCurrentTetromino.SetMovementDirection(TetrisDirection::T_LEFT);
					mCurrentTetromino.RequestMoveLeft();
				}
				else
				{
					mCurrentTetromino.UnsetMovementDirection(TetrisDirection::T_LEFT);
					mCurrentTetromino.RequestMoveLeft();
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
				if (GameController::IsPressed(state))
				{
					mCurrentTetromino.SetMovementDirection(TetrisDirection::T_RIGHT);
					mCurrentTetromino.RequestMoveRight();
				}
				else
				{
					mCurrentTetromino.UnsetMovementDirection(TetrisDirection::T_RIGHT);
					mCurrentTetromino.RequestMoveRight();
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
					mCurrentTetromino.RequestRotate();
				}
			}else if(IsGameOver())
			{
				std::cout << "Reset Game" << std::endl;
				if (GameController::IsPressed(state))
				{
					ResetGame();
				}
			}

		};

	controller.AddInputActionForKey(rotateAction);
}

void Tetris::Update(uint32_t dt)
{
	if (IsGameOver()) return;

	bool Left = true;
	bool Right = true;

	for(auto& rect : mCurrentTetromino.GetTetrisRects())
	{
		int col, row;
		if(WorldToBoard(rect.GetTopLeftPoint(), col, row))
		{
			if(row + 1 < BOARD_ROWS && mBoardOccupied[row + 1][col] )
			{
				mCurrentTetromino.SetCanMoveDown(false);
			}
			if(col + 1 < BOARD_COLS && mBoardOccupied[row][col + 1])
			{
				Right = false;
			}
			if (col - 1 >= 0 && mBoardOccupied[row][col - 1])
			{
				Left = false;
			}
		}

	}

	mCurrentTetromino.SetCanMoveLeft(Left);
	mCurrentTetromino.SetCanMoveRight(Right);

	mCurrentTetromino.Update(dt);

	if (!mCurrentTetromino.GetCanMoveDown())
	{

		for(auto& rect : mCurrentTetromino.GetTetrisRects())
		{
			int col, row;
			if (WorldToBoard(rect.GetTopLeftPoint(), col, row))
			{
				if (row <= 3) mGameState = TT_IN_GAME_OVER;
				mBoardOccupied[row][col] = true;
				mBoard[row][col] = mCurrentTetromino.GetColor();

			}
		}

		if (IsGameOver()) return;

		Tetromino OldShape = mCurrentTetromino;

		mCurrentTetromino = Tetromino({ mNextShapeType, Vec2D(20,20) });
		mNextShapeType = GetRandomShape();
		mNextShapeWindow.ChangeShape(mNextShapeType);

		CheckAndRemoveLine();
	}
}

void Tetris::Draw(Screen& screen)
{
	screen.Draw(mLevelBoundary.GetAARectangle(), Color::White(), false);

	mCurrentTetromino.Draw(screen);
	mNextShapeWindow.Draw(screen);

	Vec2D origin = mLevelBoundary.GetAARectangle().GetTopLeftPoint();

	for (int row = 0; row < BOARD_ROWS; row++)
	{
		for (int col = 0; col < BOARD_COLS; col++)
		{
			if (mBoardOccupied[row][col])
			{
				AARectangle cell = {
					origin + Vec2D(col * CELL_SIZE + BOARD_OFFSET_X, row * CELL_SIZE - BOARD_OFFSET_Y),
					CELL_SIZE, CELL_SIZE
				};
				screen.Draw(cell, Color::White(), true, mBoard[row][col]);
			}
		}
	}
}

const std::string& Tetris::GetName() const
{
	static std::string name = "Tetris <>";
	return name;
}

bool Tetris::WorldToBoard(const Vec2D& pixelPos, int& outCol, int& outRow)
{

	Vec2D origin = mLevelBoundary.GetAARectangle().GetTopLeftPoint();

	outCol = static_cast<int>((pixelPos.GetX() - origin.GetX() - BOARD_OFFSET_X) / CELL_SIZE);
	outRow = static_cast<int>((pixelPos.GetY() - origin.GetY() + BOARD_OFFSET_Y) / CELL_SIZE);

	// Guard: reject out-of-bounds
	return outCol >= 0 && outCol < BOARD_COLS &&
		outRow >= 0 && outRow < BOARD_ROWS;

}

void Tetris::ResetGame()
{
	AARectangle levelBoundary = { Vec2D::Zero, PLAYEBALE_AREA_WIDTH, PLAYEBALE_AREA_HEIGHT };

	mLevelBoundary = { levelBoundary };
	mCurrentTetromino.InitLevelBoundary(levelBoundary);

	if(IsGameOver())
	{
		for (int i = 0; i < BOARD_ROWS; i++)
		{
			for (int j = 0; j < BOARD_COLS; j++)
			{
				mBoard[i][j] = Color::Black();
				mBoardOccupied[i][j] = false;
			}
		}

		Tetromino OldShape = mCurrentTetromino;

		mCurrentTetromino = Tetromino({ mNextShapeType, Vec2D(20,20) });
		mNextShapeType = GetRandomShape();
		mNextShapeWindow.ChangeShape(mNextShapeType);

		mGameState = TT_IN_PLAY;
	}
}

bool Tetris::IsGameOver() const
{
	return mGameState == TT_IN_GAME_OVER;
}

void Tetris::CheckAndRemoveLine()
{
	bool CompletedLine = false;
	bool ThereIsACompletedLine = false;
	int RemoveRow = 100;

	for(int i = 0; i < BOARD_ROWS; i++)
	{
		for(int j = 0; j < BOARD_COLS; j++)
		{
			if(mBoardOccupied[i][j] == true)
			{
				CompletedLine = true;
				RemoveRow = i;
			}else
			{
				CompletedLine = false;
				break;
			}

		}

		if(CompletedLine)
		{
			ThereIsACompletedLine = true;
		}
	}

	if(RemoveRow < 100 && ThereIsACompletedLine)
	{
		for (int i = 0; i < BOARD_COLS; i++)
		{
			mBoardOccupied[RemoveRow][i] = false;
			mBoard[RemoveRow][i].SetRGBA(0,0,0,0);
		}

		for (int i = RemoveRow; i > 0; i--)
		{

			for (int j = 0; j < BOARD_COLS; j++)
			{

				mBoardOccupied[i][j] = mBoardOccupied[i - 1][j];
				mBoard[i][j] = mBoard[i - 1][j];

			}

		}
		
		CheckAndRemoveLine();
	}

	
}

TetrisShapeType Tetris::GetRandomShape()
{
	int Length = static_cast<int>(TetrisShapeType::Count);

	std::random_device r;
	std::default_random_engine el(r());
	std::uniform_int_distribution<int> uniform_dist(0, Length - 1);

	int randomNumber = uniform_dist(el);

	return static_cast<TetrisShapeType>(randomNumber);

	//return TetrisShapeType::TT_I;
}