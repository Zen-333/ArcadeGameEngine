#pragma once

#include "Game.h"
#include "LevelBoundary.h"
#include <vector>
#include "Tetromino.h"
#include "ShapeWindow.h"

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

private:

	bool WorldToBoard(const Vec2D& pixelPos, int& outCol, int& outRow);
	void ResetGame();
	bool IsGameOver() const;
	bool CanRotate();
	void CheckAndRemoveLine();
	TetrisShapeType GetRandomShape();

	void ResetGhostPos();
	void MoveGhostDown();
	void RotateGhost();

	Tetromino mCurrentTetromino;
	TetrisShapeType mNextShapeType;
	ShapeWindow mNextShapeWindow;
	TetrisShapes mGhostTetromino;

	TetrisGameStates mGameState;
	LevelBoundary mLevelBoundary;

	Vec2D SHAPE_START_POS;

	static constexpr int BOARD_COLS = 14;
	static constexpr int BOARD_ROWS = 28;

	static constexpr int BOARD_OFFSET_X = 5;
	static constexpr int BOARD_OFFSET_Y = 5;

	Color mBoard[BOARD_ROWS][BOARD_COLS] = {};
	bool mBoardOccupied[BOARD_ROWS][BOARD_COLS] = {};
	static constexpr int CELL_SIZE = 10;

	std::vector<AARectangle> mTetrisShapeMap[10][25];

	const uint32_t PLAYEBALE_AREA_WIDTH = 150;
	const uint32_t PLAYEBALE_AREA_HEIGHT = 280;

};