#include "ShapeWindow.h"
#include "Screen.h"
#include "Color.h"

ShapeWindow::ShapeWindow(const unsigned int Width, const unsigned int Height, const Vec2D Pos, TetrisShapeType ShapeType) :
	mWidth(Width), mHeight(Height), mRectPos(Pos),
	mShapePos(Pos.GetX() + (Width / 2) - 5, Pos.GetY() + (Height / 2) - 5), 
	mShapeType(ShapeType), mTetrisShape({ShapeType, mShapePos})
{
	mRect = {mRectPos, mWidth, mHeight};
}

void ShapeWindow::Draw(Screen& screen)
{
	screen.Draw(mRect, Color::White(), false);
	mTetrisShape.Draw(screen, Color::White());
}

void ShapeWindow::ChangeShape(TetrisShapeType NewShapeType)
{
	mShapeType = NewShapeType;
	mTetrisShape = TetrisShapes{ NewShapeType, mShapePos };
}
