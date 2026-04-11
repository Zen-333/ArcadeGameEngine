#include "Tetromino.h"

#include "BoundaryEdge.h"
#include "Utils.h"
#include "Screen.h"


Tetromino::Tetromino(TetrisShapeType InType, Vec2D InStartPos, bool Movable) : mMovable(Movable), mTetrisShape({InType, InStartPos})
{
}

void Tetromino::Update(uint32_t dt)
{

	if (!mCanMoveDown || !mMovable) return;

	bool bCanMoveLeft = true;
	bool bCanMoveRight = true;

	float dtSec = MillisecondsToSeconds(dt);
	mFallTimer += dtSec;

	if (mFallTimer >= FALL_INTERVAL)
	{
		mTetrisShape.MoveBy(Vec2D(0, mTetrisShape.GetBoxLength()));
		mFallTimer = 0.0f;
	}

	if(mDASActive)
	{
		mDASTimer += dt;
		if(mDASTimer >= DAS_DELAY)
		{
			mDelayMoveOver = true;
			mDASActive = false;
			mDASTimer = 0.0f;
			return;
		}
		mDelayMoveOver = false;
	}


	for (auto& Rect : GetTetrisRects())
	{
		if (IsGreaterThanOrEual(mBoundary.GetTopLeftPoint().GetX(), Rect.GetTopLeftPoint().GetX() - 5))
		{
			bCanMoveLeft = false;

		}
		else if (IsGreaterThanOrEual(Rect.GetBottomRightPoint().GetX() + 5, mBoundary.GetBottomRightPoint().GetX()))
		{
			bCanMoveRight = false;
		}

		if (IsGreaterThanOrEual(Rect.GetBottomRightPoint().GetY(), mBoundary.GetBottomRightPoint().GetY() - 5))
		{
			mCanMoveDown = false;
			return;
		}
	}

	if (mMoveLeftRequested && bCanMoveLeft && AllowedLeft)
	{
		mDASActive = true;
		if(mDelayMoveOver) 	mTetrisShape.MoveBy(Vec2D(-10, 0));
		mMoveLeftRequested = false;
	}
	else if (mMoveRightRequested && bCanMoveRight && AllowedRight)
	{
		mDASActive = true;
		if (mDelayMoveOver) mTetrisShape.MoveBy(Vec2D(10, 0));
		mMoveRightRequested = false;
	}

	if (mRotateRequested && mTetrisShape.GetType() != TT_O)
	{

		mTetrisShape.Rotate();
		mRotateRequested = false;
	}

}

void Tetromino::Draw(Screen& screen)
{
	mTetrisShape.Draw(screen, Color::Black());
}

void Tetromino::InitLevelBoundary(const AARectangle& Boundary)
{
	mBoundary = Boundary;
}

void Tetromino::operator=(const Tetromino& TetrisShape)
{
	mTetrisShape.SetType(TetrisShape.GetType());
	mTetrisShape.SetStartPos(TetrisShape.GetStartPos());

	mMoveLeftRequested = false;
	mMoveRightRequested = false;
	mRotateRequested = false;
	mCanMoveDown = true;
	mFallTimer = 0.0f;

	mTetrisShape.Reset();
}

