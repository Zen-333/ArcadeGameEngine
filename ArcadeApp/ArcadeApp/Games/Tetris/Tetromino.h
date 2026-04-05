#pragma once

#include <vector>
#include <unordered_map>
#include "AARectangle.h"
#include "TetrisShapes.h"
#include "Color.h"

class Screen;

class Tetromino
{
public:

	Tetromino(TetrisShapeType InType, Vec2D InStartPos, bool Movable = true);
	inline std::vector<AARectangle> GetTetrisRects() { return mTetrisShape.GetShapeRects(); }

	void Update(uint32_t dt);

	void Draw(Screen& screen);
	void InitLevelBoundary(const AARectangle& Boundary);

	inline void SetMovementDirection(TetrisDirection dir) { mTetrisShape.SetMovementDirection(dir); }; // research |
	inline void UnsetMovementDirection(TetrisDirection dir) { mTetrisShape.UnsetMovementDirection(dir); }; // Research
	inline void StopMovement() { mTetrisShape.SetDirection(0); }

	void RequestMoveLeft() { mMoveLeftRequested = true; }
	void RequestMoveRight() { mMoveRightRequested = true; }
	void RequestRotate() { mRotateRequested = true; }

	void SetCanMoveLeft(const bool CanLeft) { AllowedLeft = CanLeft; };
	void SetCanMoveRight(const bool CanRight) { AllowedRight = CanRight; };
	void SetMovable(const bool CanMove) { mMovable = CanMove; };
	void SetStartPos(const Vec2D StartPos) { mTetrisShape.SetStartPos(StartPos); };

	const bool GetCanMoveDown() { return mCanMoveDown; }
	void SetCanMoveDown(const bool MoveDown) { mCanMoveDown = MoveDown; }
	Color GetColor() const { return mTetrisShape.GetColor(); }
	TetrisShapeType GetType() const { return mTetrisShape.GetType(); };
	Vec2D GetStartPos() const { return mTetrisShape.GetStartPos(); };

	void operator=(const Tetromino& TetrisShape);

private:

	TetrisShapes mTetrisShape;

	bool AllowedLeft = true;
	bool AllowedRight = true;
	bool mMovable;

	AARectangle mBoundary;

	const float VELOCITY = 5.0f;

	bool mMoveLeftRequested = false;
	bool mMoveRightRequested = false;
	bool mRotateRequested = false;
	bool mCanMoveDown = true;
	float mFallTimer = 0.0f;

	static constexpr float FALL_INTERVAL = 0.5f;

};