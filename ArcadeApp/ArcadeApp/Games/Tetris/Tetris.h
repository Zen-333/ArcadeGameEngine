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

class Tetris : public Game
{

public:

	Tetris();

	virtual void Init(GameController& controller) override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& screen) override;
	virtual const std::string& GetName() const override;

	bool WorldToBoard(const Vec2D& pixelPos, int& outCol, int& outRow);

private:

	void ResetGame();
	bool IsGameOver() const;
	TetrisShapeType GetRandomShape();

	TetrisShapes mCurrentTetrisShape;

	TetrisGameStates mGameState;
	LevelBoundary mLevelBoundary;

	Vec2D SHAPE_START_POS;

	static constexpr int BOARD_COLS = 15;
	static constexpr int BOARD_ROWS = 28;

	Color mBoard[BOARD_ROWS][BOARD_COLS];
	bool mBoardOccupied[BOARD_ROWS][BOARD_COLS];
	static constexpr int CELL_SIZE = 10;

	Vec2D SHAPE_START_POS;
	std::vector<TetrisShapes> mArrayTetrisShapes;
	std::vector<AARectangle> mTetrisShapeMap[10][25];

	const uint32_t PLAYEBALE_AREA_WIDTH = 150;
	const uint32_t PLAYEBALE_AREA_HEIGHT = 280;

};