#include "TetrisShapes.h"
#include "BoundaryEdge.h"
#include "Utils.h"
#include "Screen.h"
#include "Color.h"



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

void TetrisShapes::Update(uint32_t dt)
{
	bool bCanMoveLeft = true;
	bool bCanMoveRight = true;

	float dtSec = MillisecondsToSeconds(dt);
	mFallTimer += dtSec;

	if (mFallTimer >= FALL_INTERVAL)
	{
		MoveBy(Vec2D(0, BOX_HEIGHT));
		mFallTimer = 0.0f;
	}


	const AARectangle& aaRect = mTetrisShapes[mTetrisShapes.size() - 1];

	for (auto& Rect : mTetrisShapes)
	{
		if (IsGreaterThanOrEual(mBoundary.GetTopLeftPoint().GetX(), Rect.GetTopLeftPoint().GetX() + (-10)))
		{
			bCanMoveLeft = false;
		}
		else if (IsGreaterThanOrEual(Rect.GetBottomRightPoint().GetX(), mBoundary.GetBottomRightPoint().GetX() + 10))
		{
			bCanMoveRight = false;
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

	if (mRotateRequested)
	{
		mTetrisShapes[0].MoveBy(Vec2D(10, 10));
		//mTetrisShapes[1].MoveBy();
		mTetrisShapes[2].MoveBy(Vec2D(-10, 10));
		mTetrisShapes[3].MoveBy(Vec2D(-20, 0));
		
		mRotateRequested = false;
	}

}

void TetrisShapes::Draw(Screen& screen)
{
	for (auto& rect : mTetrisShapes)
	{
		screen.Draw(rect, Color::Black(), true, Color::Cyan());
	}

	screen.Draw(mTetrisShapes[3], Color::Black(), true, Color::Red());

}

void TetrisShapes::InitLevelBoundary(const AARectangle& Boundary)
{
	mBoundary = Boundary;
}

void TetrisShapes::SShape()
{

	//mTetrisShapes.clear();

	//// S shape layout:
	////   [1][2]
	//// [3][4]

	//mTetrisShapes.push_back({ mTopLeftPoint + Vec2D(10 * 1, 0),      10, 10 });
	//mTetrisShapes.push_back({ mTopLeftPoint + Vec2D(10 * 2, 0),      10, 10 });

	//mTetrisShapes.push_back({ mTopLeftPoint + Vec2D(10 * 0, 10),     10, 10 });
	//mTetrisShapes.push_back({ mTopLeftPoint + Vec2D(10 * 1, 10),     10, 10 });




	mTetrisShapes.clear();

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
}

void TetrisShapes::TShape()
{
	mTetrisShapes.clear();

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
}

void TetrisShapes::OShape()
{
	mTetrisShapes.clear();

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
	mTetrisShapes.clear();

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
}

void TetrisShapes::JShape()
{
	mTetrisShapes.clear();

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
}

void TetrisShapes::LShape()
{
	mTetrisShapes.clear();

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
}

void TetrisShapes::IShape()
{
	mTetrisShapes.clear();

	for (int i = 0; i < 4; i++)
	{
		AARectangle rect = { mTopLeftPoint + Vec2D(BOX_WIDTH * i, 0), BOX_WIDTH, BOX_HEIGHT };
		mTetrisShapes.push_back(rect);
	}
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