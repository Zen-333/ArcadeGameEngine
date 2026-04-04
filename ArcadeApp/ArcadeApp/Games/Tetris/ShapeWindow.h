#pragma once
#include "Vec2D.h"
#include "AARectangle.h"

class Screen;

class ShapeWindow
{
public:

	ShapeWindow(const unsigned int Width, const const unsigned int Height, const Vec2D Pos);

	void Draw(Screen& screen);

private:

	unsigned int mWidth;
	unsigned int mHeight;

	Vec2D mRectPos;
	Vec2D mShapePos;

	AARectangle mRect;


};
