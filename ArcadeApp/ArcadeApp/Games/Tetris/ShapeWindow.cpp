#include "ShapeWindow.h"
#include "Screen.h"
#include "Color.h"

ShapeWindow::ShapeWindow(const unsigned int Width, const unsigned int Height, const Vec2D Pos) :
	mWidth(Width), mHeight(Height), mRectPos(Pos)
{
	mRect = {mRectPos, mWidth, mHeight};
}

void ShapeWindow::Draw(Screen& screen)
{
	screen.Draw(mRect, Color::White(), false);
}
