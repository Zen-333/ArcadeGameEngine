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
	mTopLeftPoint = { mStartPos.GetX() - (BOX_LENGTH / 2), mStartPos.GetY() - (BOX_LENGTH / 2) };

	auto it = ShapeMap.find(mShapeType);

	if (it != ShapeMap.end())
	{
		auto func = it->second;
		(this->*func)();
	}

	mDirection = 0;

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

void TetrisShapes::MoveBy(const Vec2D Amount)
{
	for (auto& shape : mTetrisShapes)
	{
		shape.MoveBy(Amount);
	}

}

void TetrisShapes::Rotate()
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

void TetrisShapes::Reset()
{
	mTopLeftPoint = { mStartPos.GetX() - (BOX_LENGTH / 2), mStartPos.GetY() - (BOX_LENGTH / 2) };

	auto it = ShapeMap.find(mShapeType);

	if (it != ShapeMap.end())
	{
		auto func = it->second;
		(this->*func)();
	}

	mDirection = 0;
}

void TetrisShapes::Draw(Screen& screen, Color OutlineColor, bool NewFillColor, Color FillColor)
{
	for (auto& rect : mTetrisShapes)
	{
		if(NewFillColor)
		{
			screen.Draw(rect, OutlineColor, true, FillColor);
		}else 
		{
			screen.Draw(rect, OutlineColor, true, GetColor());
		}
	}
}

void TetrisShapes::operator=(const TetrisShapes& TetrisShape)
{
	mShapeType = TetrisShape.GetType();
	mStartPos = TetrisShape.GetStartPos();

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

			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_LENGTH * s, BOX_LENGTH), BOX_LENGTH, BOX_LENGTH };
			mTetrisShapes.push_back(rect);
			s++;

		}
		else
		{
			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_LENGTH * i, 0), BOX_LENGTH, BOX_LENGTH };
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

			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_LENGTH , BOX_LENGTH), BOX_LENGTH, BOX_LENGTH };
			mTetrisShapes.push_back(rect);

		}
		else
		{
			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_LENGTH * i, 0), BOX_LENGTH, BOX_LENGTH };
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

	AARectangle rect0 = { mTopLeftPoint, BOX_LENGTH, BOX_LENGTH };
	AARectangle rect1 = { mTopLeftPoint + Vec2D(BOX_LENGTH, 0), BOX_LENGTH, BOX_LENGTH };
	AARectangle rect2 = { mTopLeftPoint + Vec2D(0, BOX_LENGTH), BOX_LENGTH, BOX_LENGTH };
	AARectangle rect3 = { mTopLeftPoint + Vec2D(BOX_LENGTH, BOX_LENGTH), BOX_LENGTH, BOX_LENGTH };

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
			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_LENGTH * i, 0), BOX_LENGTH, BOX_LENGTH };

			mTetrisShapes.push_back(rect);

		}
		else
		{

			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_LENGTH * z, BOX_LENGTH), BOX_LENGTH, BOX_LENGTH };
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
			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_LENGTH * 2, BOX_LENGTH), BOX_LENGTH, BOX_LENGTH };
			mTetrisShapes.push_back(rect);
		}
		else
		{
			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_LENGTH * i, 0), BOX_LENGTH, BOX_LENGTH };
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
			AARectangle rect = { mTopLeftPoint + Vec2D(0, BOX_LENGTH), BOX_LENGTH, BOX_LENGTH };
			mTetrisShapes.push_back(rect);
		}
		else
		{
			AARectangle rect = { mTopLeftPoint + Vec2D(BOX_LENGTH * i, 0), BOX_LENGTH, BOX_LENGTH };
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
		AARectangle rect = { mTopLeftPoint + Vec2D(BOX_LENGTH * i, 0), BOX_LENGTH, BOX_LENGTH };
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
