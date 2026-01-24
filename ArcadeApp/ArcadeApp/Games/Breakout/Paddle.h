#pragma once

#include "Excluder.h"
#include <stdint.h>

class Ball;

// Research
class Screen;

enum PaddleDirection
{
	LEFT = 1 << 0,
	RIGHT = 1 << 1

};

class Paddle: public Excluder
{

public:

	static const uint32_t PADDLE_WIDTH = 50;
	static const uint32_t PADDLE_HEIGHT = 10;


	void Init(const AARectangle& rect, const AARectangle& boundary);

	void Update(uint32_t dt, Ball& ball);

	void Draw(Screen& screen);

	bool Bounce(Ball& ball);

	inline bool IsMovingLeft() const { return mDirection == PaddleDirection::LEFT; }
	inline bool IsMovingRight() const { return mDirection == PaddleDirection::RIGHT; }

	// ~00 -> 11
	// 0 | 1 = 1
	// 0 & 1 = 0
	// 1 & 1 = 1
	inline void SetMovementDirection(PaddleDirection dir) { mDirection |= dir; }; // research |
	inline void UnsetMovementDirection(PaddleDirection dir) { mDirection &= ~dir; }; // Research
	inline void StopMovement() { mDirection = 0; }
private:

	uint32_t mDirection;
	AARectangle mBoundary;
	const float VELOCITY = 100.0f;
	const float CORNER_BOUNCE_AMT = 0.2f;

};