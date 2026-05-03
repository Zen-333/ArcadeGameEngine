#include "Tetris.h"
#include "AARectangle.h"
#include "App.h"
#include <random>
#include "Utils.h"
#include <string>
#include "BitmapFont.h"

Tetris::Tetris() : SHAPE_START_POS(Vec2D(App::Singleton().Width() / 2, 50)),
mCurrentTetromino({ GetRandomShape(), Vec2D(20,20) }), mNextShapeType(GetRandomShape()), mNextShapeWindow(65, 65, Vec2D(155, 5), mNextShapeType), mGhostTetromino(mCurrentTetromino.GetTetrisShape()), mScoreTable(mFileName)
{
	
}

void Tetris::Init(GameController& controller)
{
	mGameState = TT_IN_PLAY;

	controller.ClearAll();

	ResetGame();

	ButtonAction SnapKeyAction;
	SnapKeyAction.Key = GameController::LockKey();
	SnapKeyAction.action = [this](uint32_t dt, InputState state) {
		if (mGameState == TT_IN_PLAY)
		{
			if (GameController::IsPressed(state))
			{
				LockGhostPiece();
			}
		}
	};
	controller.AddInputActionForKey(SnapKeyAction);

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
					if(CanRotate())
					{
						mCurrentTetromino.RequestRotate();
						ResetGhostPos();
						RotateGhost();
						MoveGhostDown();
						
					}
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

	MoveGhostDown();
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

	if((Left && mCurrentTetromino.GetTetrisShape().GetDirection() == T_LEFT) || (Right && mCurrentTetromino.GetTetrisShape().GetDirection() == T_RIGHT))
	{
		ResetGhostPos();
		MoveGhostDown();
	}

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

		CheckAndRemoveLine();
		ResetGhostPos();
		mCompletedLines = 0;

		if (IsGameOver()) return;

		Tetromino OldShape = mCurrentTetromino;

		mCurrentTetromino = Tetromino({ mNextShapeType, Vec2D(20,20) });
		mGhostTetromino.SetStartPos(Vec2D(20, 20));
		mGhostTetromino.SetType(mNextShapeType);
		mGhostTetromino.Reset();
		ResetGhostPos();
		MoveGhostDown();
		mNextShapeType = GetRandomShape();
		mNextShapeWindow.ChangeShape(mNextShapeType);

	}
}

void Tetris::Draw(Screen& screen)
{
	screen.Draw(mLevelBoundary.GetAARectangle(), Color::White(), false);

	mCurrentTetromino.Draw(screen);
	mNextShapeWindow.Draw(screen);
	mGhostTetromino.Draw(screen, Color::White(), true, Color::Black());

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

	const BitmapFont& font = App::Singleton().GetFont();

	AARectangle rect = { Vec2D::Zero, App::Singleton().Width() - 10, App::Singleton().Height() };

	std::string str = "X";
	str += std::to_string(mPoints);

	Vec2D textDrawPosition;
	textDrawPosition = font.GetDrawPosition(str, rect, BFXA_RIGHT, BFYA_CENTER);
	screen.Draw(font, str, textDrawPosition, Color::White());
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
	mCompletedLines = 0;

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
		mGhostTetromino.SetStartPos(Vec2D(20, 20));
		mGhostTetromino.SetType(mNextShapeType);
		mGhostTetromino.Reset();
		ResetGhostPos();
		MoveGhostDown();
		mNextShapeType = GetRandomShape();
		mNextShapeWindow.ChangeShape(mNextShapeType);

		mGameState = TT_IN_PLAY;
		mScore.IncreaseScore(mPoints);
		mScore.SaveScore(mScoreTable);
	}
	mPoints = 0;
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
		
		mCompletedLines++;

		if (mCompletedLines >= 4) 
		{
			mPoints += PointsPerLine * PointMultiplier;
			mCompletedLines = 0;
		}
		else {
			mPoints += PointsPerLine;
		}

		std::cout << "Points: " << mPoints << std::endl;
		std::cout << "Completed lines: " << mCompletedLines << std::endl;
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

void Tetris::ResetGhostPos()
{

	Vec2D Difference =  mCurrentTetromino.GetTetrisRects()[0].GetTopLeftPoint() - mGhostTetromino.GetShapeRects()[0].GetTopLeftPoint();

	mGhostTetromino.MoveBy(Vec2D(Difference.GetX(), Difference.GetY()));

}

void Tetris::MoveGhostDown()
{
	bool CanMoveDown = true;
	while(CanMoveDown)
	{
		for (auto& rect : mGhostTetromino.GetShapeRects())
		{
			if(IsGreaterThanOrEual(rect.GetBottomRightPoint().GetY(), mLevelBoundary.GetAARectangle().GetBottomRightPoint().GetY() - 5))
			{
				CanMoveDown = false;
				break;
			}

			int col, row;
			if (WorldToBoard(rect.GetTopLeftPoint(), col, row))
			{
				if (row + 1 < BOARD_ROWS && mBoardOccupied[row + 1][col])
				{
					CanMoveDown = false;
					break;
				}
			}
		}

		if(CanMoveDown)
		{
			mGhostTetromino.MoveBy(Vec2D(0, mGhostTetromino.GetBoxLength()));
		}
	}
}

void Tetris::RotateGhost()
{
	mGhostTetromino.Rotate();
}

void Tetris::LockGhostPiece()
{
	for (auto& rect : mGhostTetromino.GetShapeRects())
	{
		int col, row;
		if (WorldToBoard(rect.GetTopLeftPoint(), col, row))
		{
			if (row <= 3) mGameState = TT_IN_GAME_OVER;
			mBoardOccupied[row][col] = true;
			mBoard[row][col] = mCurrentTetromino.GetColor();

		}
	}

	Tetromino OldShape = mCurrentTetromino;

	mCurrentTetromino = Tetromino({ mNextShapeType, Vec2D(20,20) });
	mGhostTetromino.SetStartPos(Vec2D(20, 20));
	mGhostTetromino.SetType(mNextShapeType);
	mGhostTetromino.Reset();
	mNextShapeType = GetRandomShape();
	mNextShapeWindow.ChangeShape(mNextShapeType);

	CheckAndRemoveLine();
	ResetGhostPos();
	MoveGhostDown();
	mCompletedLines = 0;


}


bool Tetris::CanRotate()
{
	if (mCurrentTetromino.GetType() == TT_O) return false;

	std::vector<AARectangle> rotation = mCurrentTetromino.GetTetrisRects();
	std::vector<AARectangle> CurrentShapeRects = mCurrentTetromino.GetTetrisRects();

	for (int i = 0; i < CurrentShapeRects.size(); i++)
	{
		Vec2D Movement = mCurrentTetromino.GetTetrisShape().GetTetrisRotations(mCurrentTetromino.GetTetrisShape().GetShapeRotationState(), i);
		rotation[i].MoveBy(mCurrentTetromino.GetTetrisShape().GetTetrisRotations(mCurrentTetromino.GetTetrisShape().GetShapeRotationState(), i));
	}

	for (auto& Rect : rotation)
	{
		int col, row;
		if (!WorldToBoard(Rect.GetTopLeftPoint(), col, row)) return false;
		if (mBoardOccupied[row][col]) return false;

		if (IsGreaterThanOrEual(mLevelBoundary.GetAARectangle().GetTopLeftPoint().GetX(), Rect.GetTopLeftPoint().GetX() + (-10)))
		{
			return false;
		}
		else if (IsGreaterThanOrEual(Rect.GetBottomRightPoint().GetX() + 10, mLevelBoundary.GetAARectangle().GetBottomRightPoint().GetX()))
		{
			return false;
		}

		if (IsGreaterThanOrEual(Rect.GetBottomRightPoint().GetY(), mLevelBoundary.GetAARectangle().GetBottomRightPoint().GetY() + 10))
		{
			return false;
		}
	}

	return true;
}