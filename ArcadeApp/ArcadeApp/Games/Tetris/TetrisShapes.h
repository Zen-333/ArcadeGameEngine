#pragma once
#include <vector>
#include <unordered_map>
#include "AARectangle.h"
#include "Color.h"

class Screen;

enum TetrisShapeType
{
	TT_S,
	TT_I,
	TT_L,
	TT_Z,
	TT_J,
	TT_O,
	TT_T,
	Count
};

enum TetrisDirection
{
	T_LEFT = 1 << 0,
	T_RIGHT = 1 << 1

};

class TetrisShapes
{
public:

	TetrisShapes(TetrisShapeType InType, Vec2D InStartPos);
	inline std::vector<AARectangle> GetTetrisShape() { return mTetrisShapes; }

	TetrisShapeType GetType() const { return mShapeType; };
	uint32_t GetDirection() const { return mDirection; };
	uint32_t GetBoxLength() const { return BOX_LENGTH; };
	Color GetColor() const { return mColor; };
	std::vector<AARectangle> GetShapeRects() const { return mTetrisShapes; };
	Vec2D GetStartPos() const { return mStartPos; };
	int GetShapeRotationState() const { return mShapeRotationState; }
	Vec2D GetTetrisRotations(int x, int y) { return mTetrisRotations[x][y]; }

	void SetDirection(uint32_t NewDirection) { mDirection = NewDirection; };
	void SetType(TetrisShapeType NewType) { mShapeType = NewType; };
	void SetStartPos(Vec2D NewStartPos) { mStartPos = NewStartPos; };
	inline void SetMovementDirection(TetrisDirection dir) { mDirection |= dir; }; // research |
	inline void UnsetMovementDirection(TetrisDirection dir) { mDirection &= ~dir; }; // Research

	void MoveBy(const Vec2D Amount);
	void Rotate();

	void Reset();

	void Draw(Screen& screen, Color OutlineColor, bool NewFillColor = false ,Color FillColor = Color::Black());
	void operator=(const TetrisShapes& TetrisShape);

private:

	void SShape();
	void TShape();
	void OShape();
	void ZShape();
	void JShape();
	void LShape();
	void IShape();

	void DefaultShapeSetup();

	static const std::unordered_map<TetrisShapeType, void (TetrisShapes::*)()> ShapeMap;

	std::vector<AARectangle> mTetrisShapes;
	TetrisShapeType mShapeType;

	const uint32_t BOX_LENGTH = 10;

	AARectangle mMiddleSquare;
	Color mColor;

	Vec2D mStartPos;
	Vec2D mTopLeftPoint;
	std::vector<Vec2D> mTetrisRotations[4];
	int mShapeRotationState = 0;

	uint32_t mDirection;

};