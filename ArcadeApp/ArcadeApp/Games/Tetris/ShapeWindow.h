#pragma once
#include "Vec2D.h"
#include "AARectangle.h"
#include "TetrisShapes.h"

class Screen;

class ShapeWindow
{
public:

	ShapeWindow(const unsigned int Width, const const unsigned int Height, const Vec2D Pos, const TetrisShapeType ShapeType);

	void Draw(Screen& screen);

	void ChangeShape(TetrisShapeType NewShapeType);

private:

	unsigned int mWidth;
	unsigned int mHeight;

	Vec2D mRectPos;
	Vec2D mShapePos;
	TetrisShapeType mShapeType;
	TetrisShapes mTetrisShape;

	AARectangle mRect;

};
