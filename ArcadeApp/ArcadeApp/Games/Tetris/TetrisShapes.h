#pragma once
#include <vector>
#include <unordered_map>
#include "AARectangle.h"


class Screen;

enum TetrisShapeType
{
	TT_S,
	TT_I,
	TT_L,
	TT_Z,
	TT_J,
	TT_O,
	TT_T
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

	void Update(uint32_t dt);

	void Draw(Screen& screen);
	void InitLevelBoundary(const AARectangle& Boundary);

	inline void SetMovementDirection(TetrisDirection dir) { mDirection |= dir; }; // research |
	inline void UnsetMovementDirection(TetrisDirection dir) { mDirection &= ~dir; }; // Research
	inline void StopMovement() { mDirection = 0; }

	void RequestMoveLeft() { mMoveLeftRequested = true; }
	void RequestMoveRight() { mMoveRightRequested = true; }
	void RequestRotate() { mRotateRequested = true; }


private:

	void SShape();
	void TShape();
	void OShape();
	void ZShape();
	void JShape();
	void LShape();
	void IShape();

	void MoveBy(const Vec2D Amount);
	void MoveTo(const Vec2D Amount);

	bool CanRotate() const;

	static const std::unordered_map<TetrisShapeType, void (TetrisShapes::*)()> ShapeMap;

	std::vector<AARectangle> mTetrisShapes;
	TetrisShapeType mShapeType;

	const uint32_t BOX_WIDTH = 10;
	const uint32_t BOX_HEIGHT = 10;

	Vec2D mStartPos;
	Vec2D mTopLeftPoint;

	// TODO: Set mBoundary
	AARectangle mBoundary;
	AARectangle mMiddleSquare;
	uint32_t mDirection;

	std::vector<Vec2D> mTetrisSRotations[4];
	int mShapeRotationState = 0;

	const float VELOCITY = 5.0f;

	bool mMoveLeftRequested = false;
	bool mMoveRightRequested = false;
	bool mRotateRequested = false;
	float mFallTimer = 0.0f;

	static constexpr float FALL_INTERVAL = 0.5f;

};