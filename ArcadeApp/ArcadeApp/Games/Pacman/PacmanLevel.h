#pragma once

#include "Vec2D.h"
#include "Excluder.h"
#include <string>
#include <vector>
#include "PacmanGameUtils.h"

class Screen;
class Pacman;

class PacmanLevel {

public:

	bool Init(const std::string& levelPath, Pacman* noptrPacman);
	void Update(uint32_t dt);
	void Draw(Screen& screen);

	bool WillCollide(const AARectangle& bbox, PacmanMovement direction) const;

private:

	struct Tile {
		Vec2D position = Vec2D::Zero;
		Vec2D offset = Vec2D::Zero;
		int width = 0;
		int collidable = 0;
		int isTeleportTile = 0;
		char teleoportToSymbol = 0;
		char symbol = '-';
	};

	bool LoadLevel(const std::string& levelPath);
	Tile* GetTileForSymbol(char symbol);

	std::vector<Excluder> mWalls;
	std::vector<Tile> mTiles;

	Vec2D mLayoutOffset;
	size_t mTileHeight;

	Pacman* mnoptrPacman;

};
