#include "TetrisShapes.h"
#include "BoundaryEdge.h"
#include "Utils.h"
#include "Screen.h"



const std::unordered_map<TetrisShapeType, void (TetrisShapes::*)()> TetrisShapes::ShapeMap =
{
	{TT_S, &TetrisShapes::SShape},
	{TT_I, &TetrisShapes::IShape},
	{TT_L, &TetrisShapes::LShape},
	{TT_Z, &TetrisShapes::ZShape},
	{TT_J, &TetrisShapes::JShape},
	{TT_O, &TetrisShapes::OShape},
	{TT_T, &TetrisShapes::TShape}
};

TetrisShapes::TetrisShapes(TetrisShapeType InType, Vec2D InStartPos) : mShapeType(InType), mStartPos(InStartPos)
{
	mTopLeftPoint = { mStartPos.GetX() - (BOX_WIDTH / 2), mStartPos.GetY() - (BOX_HEIGHT / 2) };

	auto it = ShapeMap.find(mShapeType);

	if (it != ShapeMap.end())
	{
		auto func = it->second;
		(this->*func)();
	}

	mDirection = 0;

}

void TetrisShapes::Reset()
{
	mTopLeftPoint = { mStartPos.GetX() - (BOX_WIDTH / 2), mStartPos.GetY() - (BOX_HEIGHT / 2) };

	auto it = ShapeMap.find(mShapeType);

	if (it != ShapeMap.end())
	{
		auto func = it->second;
		(this->*func)();
	}

	mDirection = 0;
}

void TetrisShapes::Update(uint32_t dt)
{

	if (!mCanMoveDown) return;

	bool bCanMoveLeft = true;
	bool bCanMoveRight = true;

	float dtSec = MillisecondsToSeconds(dt);
	mFallTimer += dtSec;

	if (mFallTimer >= FALL_INTERVAL)
	{
		MoveBy(Vec2D(0, BOX_HEIGHT));
		mFallTimer = 0.0f;
	}


	for (auto& Rect : mTetrisShapes)
	{
		if (IsGreaterThanOrEual(mBoundary.GetTopLeftPoint().GetX(), Rect.GetTopLeftPoint().GetX() - 5))
		{
			bCanMoveLeft = false;
		}
		else if (IsGreaterThanOrEual(Rect.GetBottomRightPoint().GetX(), mBoundary.GetBottomRightPoint().GetX() - 5))
		{
			bCanMoveRight = false;
		}

		if (IsGreaterThanOrEual(Rect.GetBottomRightPoint().GetY(), mBoundary.GetBottomRightPoint().GetY() - 5)) 
		{
			mCanMoveDown = false;
			return;
		}
	}

	if (mMoveLeftRequested && bCanMoveLeft)
	{
		MoveBy(Vec2D(-10, 0));
		mMoveLeftRequested = false;
	}
	else if (mMoveRightRequested && bCanMoveRight)
	{
		MoveBy(Vec2D(10, 0));
		mMoveRightRequested = false;
	}

	if (mRotateRequested && mShapeType != TT_O)
	{
		if(CanRotate())
		{
			for (int i = 0; i < mTetrisShapes.size(); i++)
			{
				Vec2D Movement = mTetrisRotations[mShapeRotationState][i];
				mTetrisShapes[i].MoveBy(mTetrisRotations[mShapeRotationState][i]);

			}

			if (mShapeRotationState == 3)
			{
				mShapeRotationState = 0;
			}
			else
			{
				mShapeRotationState++;
			}
		}

		mRotateRequested = false;
	}

}

void TetrisShapes::Draw(Screen& screen)
{
	for (auto& rect : mTetrisShapes)
	{
		screen.Draw(rect, Color::Black(), true, mColor);
	}

}

void TetrisShapes::InitLevelBoundary(const AARectangle& Boundary)
{
	mBoundary = Boundary;
}

void TetrisShapes::operator=(const TetrisShapes& TetrisShape)
{
	mShapeType = TetrisShape.mShapeType;
	mStartPos = TetrisShape.mStartPos;

	mMoveLeftRequested = false;
	mMoveRightRequested = false;
	mRotateRequested = false;
	mCanMoveDown = true;
	mFallTimer = 0.0f;

	Reset();

}

void TetrisShapes::SShape()
{

	DefaultShapeSetup();

	mColor = Color::Blue();

	int s = 1;
	for (int i = 0; i < 4; i++)
	{
		if (i < 2)
		{

			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_HEIGHT * s, BOX_WIDTH), BOX_WIDTH, BOX_HEIGHT };
			mTetrisShapes.push_back(rect);
			s++;

		}
		else
		{
			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_HEIGHT * i, 0), BOX_WIDTH, BOX_HEIGHT };
			mTetrisShapes.push_back(rect);
		}

	}

	mMiddleSquare = mTetrisShapes[1];

	// Rotation 0  (horizontal)
	mTetrisRotations[0].push_back(Vec2D(10, 10));
	mTetrisRotations[0].push_back(Vec2D(0, 0));
	mTetrisRotations[0].push_back(Vec2D(-10, 10));
	mTetrisRotations[0].push_back(Vec2D(-20, 0));

	// Rotation 1 (vertical)
	mTetrisRotations[1].push_back(Vec2D(10, -10));
	mTetrisRotations[1].push_back(Vec2D(0, 0));
	mTetrisRotations[1].push_back(Vec2D(10, 10));
	mTetrisRotations[1].push_back(Vec2D(0, 20));

	// Rotation 2 (same as 0)
	mTetrisRotations[2].push_back(Vec2D(-10, -10));
	mTetrisRotations[2].push_back(Vec2D(0, 0));
	mTetrisRotations[2].push_back(Vec2D(10, -10));
	mTetrisRotations[2].push_back(Vec2D(20, 0));

	// Rotation 3 (same as 1)
	mTetrisRotations[3].push_back(Vec2D(-10, 10));
	mTetrisRotations[3].push_back(Vec2D(0, 0));
	mTetrisRotations[3].push_back(Vec2D(-10, -10));
	mTetrisRotations[3].push_back(Vec2D(0, -20));
}

void TetrisShapes::TShape()
{
	DefaultShapeSetup();
	mColor = Color::Yellow();

	for (int i = 0; i < 4; i++)
	{
		if (i == 3)
		{

			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_HEIGHT , BOX_WIDTH), BOX_WIDTH, BOX_HEIGHT };
			mTetrisShapes.push_back(rect);

		}
		else
		{
			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_HEIGHT * i, 0), BOX_WIDTH, BOX_HEIGHT };
			mTetrisShapes.push_back(rect);
		}

	}

	mMiddleSquare = mTetrisShapes[1]; // Pivot on center block

	// Rotation 0 ? 1 (T pointing down to pointing right)
	mTetrisRotations[0].push_back(Vec2D(10, 10));
	mTetrisRotations[0].push_back(Vec2D(0, 0));
	mTetrisRotations[0].push_back(Vec2D(-10, -10));
	mTetrisRotations[0].push_back(Vec2D(10, -10));

	// Rotation 1 ? 2 (pointing right to pointing up)
	mTetrisRotations[1].push_back(Vec2D(10, -10));
	mTetrisRotations[1].push_back(Vec2D(0, 0));
	mTetrisRotations[1].push_back(Vec2D(-10, 10));
	mTetrisRotations[1].push_back(Vec2D(-10, -10));

	// Rotation 2 ? 3 (pointing up to pointing left)
	mTetrisRotations[2].push_back(Vec2D(-10, -10));
	mTetrisRotations[2].push_back(Vec2D(0, 0));
	mTetrisRotations[2].push_back(Vec2D(10, 10));
	mTetrisRotations[2].push_back(Vec2D(-10, 10));

	// Rotation 3 ? 0 (pointing left to pointing down)
	mTetrisRotations[3].push_back(Vec2D(-10, 10));
	mTetrisRotations[3].push_back(Vec2D(0, 0));
	mTetrisRotations[3].push_back(Vec2D(10, -10));
	mTetrisRotations[3].push_back(Vec2D(10, 10));
}

void TetrisShapes::OShape()
{
	DefaultShapeSetup();
	mColor = Color::Green();

	AARectangle rect0 = { mTopLeftPoint, BOX_WIDTH, BOX_HEIGHT };
	AARectangle rect1 = { mTopLeftPoint + Vec2D(BOX_HEIGHT, 0), BOX_WIDTH, BOX_HEIGHT };
	AARectangle rect2 = { mTopLeftPoint + Vec2D(0, BOX_HEIGHT), BOX_WIDTH, BOX_HEIGHT };
	AARectangle rect3 = { mTopLeftPoint + Vec2D(BOX_HEIGHT, BOX_HEIGHT), BOX_WIDTH, BOX_HEIGHT };

	mTetrisShapes.push_back(rect0);
	mTetrisShapes.push_back(rect1);
	mTetrisShapes.push_back(rect2);
	mTetrisShapes.push_back(rect3);
}

void TetrisShapes::ZShape()
{
	DefaultShapeSetup();
	mColor = Color::Cyan();

	int z = 1;
	for (int i = 0; i < 4; i++)
	{
		if (i < 2)
		{
			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_HEIGHT * i, 0), BOX_WIDTH, BOX_HEIGHT };

			mTetrisShapes.push_back(rect);

		}
		else
		{

			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_HEIGHT * z, BOX_WIDTH), BOX_WIDTH, BOX_HEIGHT };
			mTetrisShapes.push_back(rect);
			z++;
		}

	}

	mMiddleSquare = mTetrisShapes[2]; // Pivot on block [2]

	// Rotation 0 ? 1 (horizontal to vertical)
	mTetrisRotations[0].push_back(Vec2D(20, 0));
	mTetrisRotations[0].push_back(Vec2D(10, 10));
	mTetrisRotations[0].push_back(Vec2D(0, 0));
	mTetrisRotations[0].push_back(Vec2D(-10, 10));

	// Rotation 1 ? 2 (vertical to horizontal)
	mTetrisRotations[1].push_back(Vec2D(-20, 0));
	mTetrisRotations[1].push_back(Vec2D(-10, -10));
	mTetrisRotations[1].push_back(Vec2D(0, 0));
	mTetrisRotations[1].push_back(Vec2D(10, -10));

	// Rotation 2 ? 3 (same as 0)
	mTetrisRotations[2].push_back(Vec2D(20, 0));
	mTetrisRotations[2].push_back(Vec2D(10, 10));
	mTetrisRotations[2].push_back(Vec2D(0, 0));
	mTetrisRotations[2].push_back(Vec2D(-10, 10));

	// Rotation 3 ? 0 (same as 1)
	mTetrisRotations[3].push_back(Vec2D(-20, 0));
	mTetrisRotations[3].push_back(Vec2D(-10, -10));
	mTetrisRotations[3].push_back(Vec2D(0, 0));
	mTetrisRotations[3].push_back(Vec2D(10, -10));
}

void TetrisShapes::JShape()
{
	DefaultShapeSetup();
	mColor = Color::Magenta();

	for (int i = 0; i < 4; i++)
	{
		if (i == 3)
		{
			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_WIDTH * 2, BOX_WIDTH), BOX_WIDTH, BOX_HEIGHT };
			mTetrisShapes.push_back(rect);
		}
		else
		{
			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_WIDTH * i, 0), BOX_WIDTH, BOX_HEIGHT };
			mTetrisShapes.push_back(rect);
		}

	}

	mMiddleSquare = mTetrisShapes[1]; // Pivot on center

	// Rotation 0 ? 1
	mTetrisRotations[0].push_back(Vec2D(10, 10));
	mTetrisRotations[0].push_back(Vec2D(0, 0));
	mTetrisRotations[0].push_back(Vec2D(-10, -10));
	mTetrisRotations[0].push_back(Vec2D(0, -20));

	// Rotation 1 ? 2
	mTetrisRotations[1].push_back(Vec2D(10, -10));
	mTetrisRotations[1].push_back(Vec2D(0, 0));
	mTetrisRotations[1].push_back(Vec2D(-10, 10));
	mTetrisRotations[1].push_back(Vec2D(-20, 0));

	// Rotation 2 ? 3
	mTetrisRotations[2].push_back(Vec2D(-10, -10));
	mTetrisRotations[2].push_back(Vec2D(0, 0));
	mTetrisRotations[2].push_back(Vec2D(10, 10));
	mTetrisRotations[2].push_back(Vec2D(0, 20));

	// Rotation 3 ? 0
	mTetrisRotations[3].push_back(Vec2D(-10, 10));
	mTetrisRotations[3].push_back(Vec2D(0, 0));
	mTetrisRotations[3].push_back(Vec2D(10, -10));
	mTetrisRotations[3].push_back(Vec2D(20, 0));
}

void TetrisShapes::LShape()
{
	DefaultShapeSetup();
	mColor = Color::Red();

	for (int i = 0; i < 4; i++)
	{

		if (i == 3)
		{
			AARectangle rect = { mTopLeftPoint + Vec2D(0, BOX_WIDTH), BOX_WIDTH, BOX_HEIGHT };
			mTetrisShapes.push_back(rect);
		}
		else
		{
			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_WIDTH * i, 0), BOX_WIDTH, BOX_HEIGHT };
			mTetrisShapes.push_back(rect);
		}

	}

	mMiddleSquare = mTetrisShapes[1]; // Pivot on center

	// Rotation 0 ? 1
	mTetrisRotations[0].push_back(Vec2D(10, 10));
	mTetrisRotations[0].push_back(Vec2D(0, 0));
	mTetrisRotations[0].push_back(Vec2D(-10, -10));
	mTetrisRotations[0].push_back(Vec2D(20, 0));

	// Rotation 1 ? 2
	mTetrisRotations[1].push_back(Vec2D(10, -10));
	mTetrisRotations[1].push_back(Vec2D(0, 0));
	mTetrisRotations[1].push_back(Vec2D(-10, 10));
	mTetrisRotations[1].push_back(Vec2D(0, -20));

	// Rotation 2 ? 3
	mTetrisRotations[2].push_back(Vec2D(-10, -10));
	mTetrisRotations[2].push_back(Vec2D(0, 0));
	mTetrisRotations[2].push_back(Vec2D(10, 10));
	mTetrisRotations[2].push_back(Vec2D(-20, 0));

	// Rotation 3 ? 0
	mTetrisRotations[3].push_back(Vec2D(-10, 10));
	mTetrisRotations[3].push_back(Vec2D(0, 0));
	mTetrisRotations[3].push_back(Vec2D(10, -10));
	mTetrisRotations[3].push_back(Vec2D(0, 20));
}

void TetrisShapes::IShape()
{
	DefaultShapeSetup();
	mColor = Color::Orange();

	for (int i = 0; i < 4; i++)
	{
		AARectangle rect = { mTopLeftPoint + Vec2D(BOX_WIDTH * i, 0), BOX_WIDTH, BOX_HEIGHT };
		mTetrisShapes.push_back(rect);
	}

	mMiddleSquare = mTetrisShapes[1]; // Pivot between blocks 1 and 2

	// Rotation 0 ? 1 (horizontal to vertical)
	mTetrisRotations[0].push_back(Vec2D(10, -10));
	mTetrisRotations[0].push_back(Vec2D(0, 0));
	mTetrisRotations[0].push_back(Vec2D(-10, 10));
	mTetrisRotations[0].push_back(Vec2D(-20, 20));

	// Rotation 1 ? 2 (vertical to horizontal)
	mTetrisRotations[1].push_back(Vec2D(-10, 10));
	mTetrisRotations[1].push_back(Vec2D(0, 0));
	mTetrisRotations[1].push_back(Vec2D(10, -10));
	mTetrisRotations[1].push_back(Vec2D(20, -20));

	// Rotation 2 ? 3 (same as 0)
	mTetrisRotations[2].push_back(Vec2D(10, -10));
	mTetrisRotations[2].push_back(Vec2D(0, 0));
	mTetrisRotations[2].push_back(Vec2D(-10, 10));
	mTetrisRotations[2].push_back(Vec2D(-20, 20));

	// Rotation 3 ? 0 (same as 1)
	mTetrisRotations[3].push_back(Vec2D(-10, 10));
	mTetrisRotations[3].push_back(Vec2D(0, 0));
	mTetrisRotations[3].push_back(Vec2D(10, -10));
	mTetrisRotations[3].push_back(Vec2D(20, -20));
}

void TetrisShapes::MoveBy(const Vec2D Amount)
{
	for (auto& shape : mTetrisShapes)
	{
		shape.MoveBy(Amount);
	}

}

void TetrisShapes::MoveTo(const Vec2D Amount)
{

}

void TetrisShapes::DefaultShapeSetup()
{
	mTetrisShapes.clear();
	mShapeRotationState = 0;

	for (int i = 0; i < 4; i++)
	{
		mTetrisRotations[i].clear();
	}
}

bool TetrisShapes::CanRotate() const
{
	std::vector<AARectangle> rotation = mTetrisShapes;

	for (int i = 0; i < mTetrisShapes.size(); i++)
	{
		Vec2D Movement = mTetrisRotations[mShapeRotationState][i];
		rotation[i].MoveBy(mTetrisRotations[mShapeRotationState][i]);

	}

	for (auto& Rect : rotation)
	{
		if (IsGreaterThanOrEual(mBoundary.GetTopLeftPoint().GetX(), Rect.GetTopLeftPoint().GetX() + (-10)))
		{
			return false;
		}
		else if (IsGreaterThanOrEual(Rect.GetBottomRightPoint().GetX(), mBoundary.GetBottomRightPoint().GetX() + 10))
		{
			return false;
		}

		if (IsGreaterThanOrEual(Rect.GetBottomRightPoint().GetY(), mBoundary.GetBottomRightPoint().GetY() + 10))
		{
			return false;
		}
	}

	return true;
}

