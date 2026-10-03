#pragma once

#include "Vec2D.h"
#include "Excluder.h"
#include <string>
#include <vector>
#include "PacmanGameUtils.h"
#include "SpriteSheet.h"
#include <random>

class Screen;
class Pacman;
class Ghost;

class PacmanLevel {

public:

	bool Init(const std::string& levelPath,const SpriteSheet* noptrSpriteSheet);
	void Update(uint32_t dt,Pacman& pacman, std::vector<Ghost>& ghosts);
	void Draw(Screen& screen);
	void ResetLevel();

	bool WillCollide(const AARectangle& bbox, PacmanMovement direction) const;

	inline Vec2D GetLayoutOffset() const { return mLayoutOffset; }
	inline Vec2D GetPacmanSpawnLocation() const { return mPacmanSpawnLocation; }
	bool IsLevelOver() const;
	void IncreaseLevel();
	void ResetToFirstLevel();
	inline const std::vector<Vec2D>& GetGhostSpawnPoints() { return mGhostsSpawnPoints; }

private:

	struct Tile {
		Vec2D position = Vec2D::Zero;
		Vec2D offset = Vec2D::Zero;
		int width = 0;
		int collidable = 0;
		int isTeleportTile = 0;
		int excludePelletTile = 0;
		int pacmanSpawnPoint = 0;
		int itemSpawnPoint = 0;
		int blinkySpawnPoint = 0;
		int inkySpawnPoint = 0;
		int pinkySpawnPoint = 0;
		int clydeSpawnPoint = 0;
		char teleoportToSymbol = 0;
		char symbol = '-';
	};

	struct Pellet
	{
		uint32_t score = 0;
		AARectangle mBBox;
		int powerPellet = 0;
		int eaten = 0;
	};

	struct BonusItem
	{
		uint32_t score = 0;
		AARectangle bbox;
		int eaten = 0;
		int spawned = 0;
		int spawnTime = -1;
	};

	struct BonusItemLevelProperties
	{
		uint32_t score = 0;
		std::string spriteName = "";
		uint32_t begin = 0; // which level does this item spawn at
		uint32_t end = 0;
	};

	bool LoadLevel(const std::string& levelPath);
	Tile* GetTileForSymbol(char symbol);
	void ResetPellets();
	bool HasEatenAllPellets() const;
	size_t NumPelletsEaten() const;
	void GetBonusItemSpriteName(std::string& spriteName, uint32_t& score) const;
	void SpawnBonusItem();
	bool ShouldSpawnBonusItem() const;

	std::vector<Vec2D> mGhostsSpawnPoints;
	std::default_random_engine mGenerator;
	BonusItem mBonusItem;
	std::string mBonusItemSpriteName;
	const SpriteSheet* mnoptrSpriteSheet;
	std::vector<BonusItemLevelProperties> mBonusItemProperties;

	std::vector<Excluder> mWalls;
	std::vector<Tile> mTiles;

	std::vector<Tile> mExclusionTiles;
	std::vector<Pellet> mPellets;
	Vec2D mPacmanSpawnLocation;

	Vec2D mLayoutOffset;
	size_t mTileHeight;
	int mCurrentLevel;

};
