#pragma once

#include "Game.h"
#include "LevelBoundary.h"
#include <vector>
#include "TetrisShapes.h"

enum TetrisGameStates
{
	TT_IN_PLAY = 0,
	TT_IN_GAME_OVER
};

class Tetris: public Game
{

public:

	Tetris();

	virtual void Init(GameController& controller) override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& screen) override;
	virtual const std::string& GetName() const override;

private:

	void ResetGame();
	bool IsGameOver() const;

	TetrisShapes mCurrentTetrisShape;

	TetrisGameStates mGameState;
	LevelBoundary mLevelBoundary; 

	Vec2D SHAPE_START_POS;

	const uint32_t PLAYEBALE_AREA_WIDTH = 100;
	const uint32_t PLAYEBALE_AREA_HEIGHT = 270;

};