#include "Pacman/PacmanLevel.h"
#include "FileCommandLoader.h"
#include "Screen.h"
#include "Pacman.h"
#include <cassert>
#include "App.h"
#include "Circle.h"
#include "Excluder.h"

namespace
{
	const uint32_t NUM_LEVELS = 256;
	const uint32_t SPRITE_HEIGHT = 16;
	const uint32_t SPRITE_WIDTH = 16;
}

bool PacmanLevel::Init(const std::string& levelPath, const SpriteSheet* noptrSpriteSheet, Pacman* noptrPacman)
{
	mCurrentLevel = 0;
	mnoptrPacman = noptrPacman;
	mnoptrSpriteSheet = noptrSpriteSheet;
	mBonusItemSpriteName = "";
	std::random_device r;
	mGenerator.seed(r());

	bool levelLoaded = LoadLevel(levelPath);

	if(levelLoaded)
	{
		ResetLevel();
	}

	return levelLoaded;
}

void PacmanLevel::Update(uint32_t dt) 
{
	for(const auto& wall : mWalls)
	{
		BoundaryEdge edge;

		if (wall.HasCollided(mnoptrPacman->GetBoundingBox(), edge))
		{
			Vec2D offset = wall.GetCollisionOffset(mnoptrPacman->GetBoundingBox());
			mnoptrPacman->MoveBy(offset);
			mnoptrPacman->Stop();
		}
	}

	for(Tile t: mTiles)
	{
		if(t.isTeleportTile)
		{
			AARectangle teleportTileAABB(t.position, t.width, static_cast<float>(mTileHeight));
			
			Tile* teleportToTile = GetTileForSymbol(t.teleoportToSymbol);
			assert(teleportToTile);

			if(teleportToTile->isTeleportTile && teleportTileAABB.Intersects(mnoptrPacman->GetBoundingBox()))
			{
				mnoptrPacman->MoveTo(teleportToTile->position + teleportToTile->offset);
			}
		}
	}

	for(auto& pellet: mPellets)
	{
		if(!pellet.eaten)
		{
			if(mnoptrPacman->GetEactingBoundingBox().Intersects(pellet.mBBox))
			{
				pellet.eaten = true;

				mnoptrPacman->AteItem(pellet.score);

				if(pellet.powerPellet)
				{
					mnoptrPacman->ResetGhostEatenMultiplier();
					// TODO: make ghosts go vulnerable
				}
			}
		}
	}

	if(ShouldSpawnBonusItem())
	{
		SpawnBonusItem();
	}

	if(mBonusItem.spawned && !mBonusItem.eaten)
	{
		if(mnoptrPacman->GetEactingBoundingBox().Intersects(mBonusItem.bbox))
		{
			mBonusItem.eaten = true;
			mnoptrPacman->AteItem(mBonusItem.score);
		}
	}
}

void PacmanLevel::Draw(Screen& screen)
{
	for(auto& wall: mWalls)
	{
		screen.Draw(wall.GetAARectangle(), Color::Blue());
	}

	for(const auto& pellet: mPellets)
	{
		if(!pellet.eaten)
		{
			if(!pellet.powerPellet)
			{
				screen.Draw(pellet.mBBox, Color::White());
			}else
			{
				Circle c(pellet.mBBox.GetCenterPoint(), pellet.mBBox.GetWidth() / 2.0f);
				screen.Draw(c, Color::White(), true, Color::White());
			}
		}
	}

	if(mBonusItem.spawned && !mBonusItem.eaten)
	{
		screen.Draw(*mnoptrSpriteSheet, mBonusItemSpriteName, mBonusItem.bbox.GetTopLeftPoint());
	}
}

bool PacmanLevel::WillCollide(const AARectangle& aBBox, PacmanMovement direction) const
{
	AARectangle bbox = aBBox;

	bbox.MoveBy(GetMovementVector(direction));

	for(const auto& wall : mWalls)
	{
		BoundaryEdge edge;

		if(wall.HasCollided(bbox, edge))
		{
			return true;
		}
	}

	return false;

}

bool PacmanLevel::ShouldSpawnBonusItem() const 
{
	auto numEaten = NumPelletsEaten();
	return !mBonusItem.spawned && numEaten >= mBonusItem.spawnTime;
}

void PacmanLevel::SpawnBonusItem()
{
	mBonusItem.spawned = 1;
	mBonusItem.eaten = 0;
	
}

void PacmanLevel::GetBonusItemSpriteName(std::string& spriteName, uint32_t& score) const
{
	for(const auto& properties : mBonusItemProperties)
	{
		if(mCurrentLevel >= properties.begin && mCurrentLevel <= properties.end)
		{
			spriteName = properties.spriteName;
			score = properties.score;
			return;
		}
	}
}

void PacmanLevel::ResetLevel()
{
	ResetPellets();

	std::uniform_int_distribution<size_t> distribution(20, mPellets.size() - 50);
	mBonusItem.spawnTime = static_cast<int>(distribution(mGenerator));

	GetBonusItemSpriteName(mBonusItemSpriteName, mBonusItem.score);

	if(mnoptrPacman)
	{
		mnoptrPacman->MoveTo(mPacmanSpawnLocation);
		mnoptrPacman->ResetToFirstAnimation();
	}
}

bool PacmanLevel::IsLevelOver() const
{
	return HasEatenAllPellets();
}

void PacmanLevel::IncreaseLevel()
{
	mCurrentLevel++;

	if(mCurrentLevel > NUM_LEVELS)
	{
		mCurrentLevel = 1;
	}

	ResetLevel();
}

void PacmanLevel::ResetToFirstLevel()
{
	mCurrentLevel = 1;
	ResetLevel();
}

bool PacmanLevel::HasEatenAllPellets() const
{
	return NumPelletsEaten() >= mPellets.size() - 4; // 4 super pellets
}

size_t PacmanLevel::NumPelletsEaten() const 
{
	size_t numEaten = 0;

	for(const auto& pellet: mPellets)
	{
		if(!pellet.powerPellet && pellet.eaten)
		{
			numEaten++;
		}
	}

	return numEaten;
}

void PacmanLevel::ResetPellets()
{
	mPellets.clear();

	const uint32_t PELLET_SIZE = 2;
	const uint32_t PADDING = static_cast<uint32_t>(mTileHeight);

	uint32_t startingY = mLayoutOffset.GetY() + PADDING + mTileHeight - 1;
	uint32_t startingX = PADDING + 3;

	const uint32_t LEVEL_HEIGHT = mLayoutOffset.GetY() + 32 * mTileHeight;

	Pellet p;
	p.score = 10;

	uint32_t row = 0;

	for(uint32_t y = startingY; y < LEVEL_HEIGHT; y += PADDING, row++)
	{
		for(uint32_t x = startingX, col = 0; x < App::Singleton().Width(); x += PADDING, col++)
		{
			if(row == 0  ||  row == 22)
			{
				if(col == 0 || col == 25)
				{
					p.powerPellet = 1;
					p.score = 50;
					p.mBBox = AARectangle(Vec2D(x - 3, y - 3), mTileHeight, mTileHeight);
					mPellets.push_back(p);

					p.powerPellet = 0;
					p.score = 10;

					continue;
				}
			}

			AARectangle rect = AARectangle(Vec2D(x, y), PELLET_SIZE, PELLET_SIZE);

			bool found = false;

			for(Excluder& wall: mWalls)
			{
				if(wall.GetAARectangle().Intersects(rect))
				{
					found = true;
					break;
				}
			}

			if(!found)
			{
				for(const Tile& excludedPelletTile: mExclusionTiles)
				{
					if(excludedPelletTile.excludePelletTile)
					{
						AARectangle tileAABB(excludedPelletTile.position, excludedPelletTile.width, mTileHeight);

						if(tileAABB.Intersects(rect))
						{
							found = true;
							break;
						}
					}
				}
			}
			if(!found)
			{
				p.mBBox = rect;
				mPellets.push_back(p);
			}
		}
	}
}

bool PacmanLevel::LoadLevel(const std::string& levelPath)
{
	FileCommandLoader fileLoader;

	Command tileWidthCommand;
	tileWidthCommand.command = "tile_width";
	tileWidthCommand.parseFunc = [this](ParseFuncParams params) 
	{
			mTiles.back().width = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileWidthCommand);

	Command tileHeightCommand;
	tileHeightCommand.command = "tile_height";
	tileHeightCommand.parseFunc = [this](ParseFuncParams params)
		{
			mTileHeight = FileCommandLoader::ReadInt(params);
		};
	fileLoader.AddCommand(tileHeightCommand);

	Command tileCommand;
	tileCommand.command = "tile";
	tileCommand.parseFunc = [this](ParseFuncParams params)
		{
			mTiles.push_back(Tile());
		};
	fileLoader.AddCommand(tileCommand);

	Command tileSymbolCommand;
	tileSymbolCommand.command = "tile_symbol";
	tileSymbolCommand.parseFunc = [this](ParseFuncParams params)
		{
			mTiles.back().symbol = FileCommandLoader::ReadChar(params);
		};
	fileLoader.AddCommand(tileSymbolCommand);

	Command tileCollisionCommand;
	tileCollisionCommand.command = "tile_collision";
	tileCollisionCommand.parseFunc = [this](ParseFuncParams params)
		{
			mTiles.back().collidable = FileCommandLoader::ReadInt(params);
		};
	fileLoader.AddCommand(tileCollisionCommand);

	Vec2D layoutOffset;

	Command layoutOffsetCommand;
	layoutOffsetCommand.command = "layout_offset";
	layoutOffsetCommand.parseFunc = [&layoutOffset, this](ParseFuncParams params)
	{
			mLayoutOffset = FileCommandLoader::ReadSize(params);
			layoutOffset = mLayoutOffset;
	};
	fileLoader.AddCommand(layoutOffsetCommand);


	Command tileToTeleportToCommand;
	tileToTeleportToCommand.command = "tile_teleport_to_symbol";
	tileToTeleportToCommand.parseFunc = [this](ParseFuncParams params)
		{
			mTiles.back().teleoportToSymbol = FileCommandLoader::ReadChar(params);
		};
	fileLoader.AddCommand(tileToTeleportToCommand);

	Command tileIsTeleportTileCommand;
	tileIsTeleportTileCommand.command = "tile_is_teleport_tile";
	tileIsTeleportTileCommand.parseFunc = [this](ParseFuncParams params)
		{
			mTiles.back().isTeleportTile = FileCommandLoader::ReadInt(params);
		};
	fileLoader.AddCommand(tileIsTeleportTileCommand);

	Command tileOffsetCommand;
	tileOffsetCommand.command = "tile_offset";
	tileOffsetCommand.parseFunc = [this](ParseFuncParams params)
		{
			mTiles.back().offset = FileCommandLoader::ReadSize(params);
		};
	fileLoader.AddCommand(tileOffsetCommand);

	Command tileExcludePelletCommand;
	tileExcludePelletCommand.command = "tile_exclude_pellet";
	tileExcludePelletCommand.parseFunc = [this](ParseFuncParams params)
		{
			mTiles.back().excludePelletTile = FileCommandLoader::ReadInt(params);
		};
	fileLoader.AddCommand(tileExcludePelletCommand);

	Command tilePacmanSpawnPointCommand;
	tilePacmanSpawnPointCommand.command = "tile_pacman_spawn_point";
	tilePacmanSpawnPointCommand.parseFunc = [this](ParseFuncParams params) 
		{
			mTiles.back().pacmanSpawnPoint = FileCommandLoader::ReadInt(params);
		};
	fileLoader.AddCommand(tilePacmanSpawnPointCommand);

	Command tileItemSpawnPointCommand;
	tileItemSpawnPointCommand.command = "tile_item_spawn_point";
	tileItemSpawnPointCommand.parseFunc = [this](ParseFuncParams params)
		{
			mTiles.back().itemSpawnPoint = FileCommandLoader::ReadInt(params);
		};
	fileLoader.AddCommand(tileItemSpawnPointCommand);

	Command layoutCommand;
	layoutCommand.command = "layout";
	layoutCommand.commandType = COMMAND_MULTI_LINE;
	layoutCommand.parseFunc = [&layoutOffset, this](ParseFuncParams params)
		{
			int startingX = layoutOffset.GetX();

			for(int c = 0; c < params.line.length(); c++)
			{
				Tile* tile = GetTileForSymbol(params.line[c]);

				if(tile)
				{
					tile->position = Vec2D(startingX, layoutOffset.GetY());

					if(tile->collidable > 0)
					{
						Excluder wall;
						wall.Init(AARectangle(Vec2D(startingX, layoutOffset.GetY()), tile->width, static_cast<int>(mTileHeight)));
						mWalls.push_back(wall);
					}

					if(tile->pacmanSpawnPoint > 0)
					{
						mPacmanSpawnLocation = Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY());

					}else if(tile->itemSpawnPoint > 0)
					{
						mBonusItem.bbox = AARectangle(Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY()), SPRITE_WIDTH, SPRITE_HEIGHT);

					}

					if(tile->excludePelletTile > 0)
					{
						mExclusionTiles.push_back(*tile);
					}

					startingX += tile->width;
				}
			}
			layoutOffset += Vec2D(0, mTileHeight);
		};
	fileLoader.AddCommand(layoutCommand);

	Command bonusItemCommand;
	bonusItemCommand.command = "bonus_item";
	bonusItemCommand.parseFunc = [this](ParseFuncParams params)
		{
			BonusItemLevelProperties newProperty;
			mBonusItemProperties.push_back(newProperty);
		};
	fileLoader.AddCommand(bonusItemCommand);

	Command bonusItemSpriteNameCommand;
	bonusItemSpriteNameCommand.command = "bonus_item_sprite_name";
	bonusItemSpriteNameCommand.parseFunc = [this](ParseFuncParams params)
		{
			mBonusItemProperties.back().spriteName = FileCommandLoader::ReadString(params);
		};
	fileLoader.AddCommand(bonusItemSpriteNameCommand);

	Command bonusItemScoreCommand;
	bonusItemScoreCommand.command = "bonus_item_score";
	bonusItemScoreCommand.parseFunc = [this](ParseFuncParams params)
		{
			mBonusItemProperties.back().score = FileCommandLoader::ReadInt(params);
		};
	fileLoader.AddCommand(bonusItemScoreCommand);

	Command bonusItemBeginLevelCommand;
	bonusItemBeginLevelCommand.command = "bonus_item_begin_level";
	bonusItemBeginLevelCommand.parseFunc = [this](ParseFuncParams params)
		{
			mBonusItemProperties.back().begin = FileCommandLoader::ReadInt(params);
		};
	fileLoader.AddCommand(bonusItemBeginLevelCommand);

	Command bonusItemEndLevelCommand;
	bonusItemEndLevelCommand.command = "bonus_item_end_level";
	bonusItemEndLevelCommand.parseFunc = [this](ParseFuncParams params)
		{
			mBonusItemProperties.back().end = FileCommandLoader::ReadInt(params);
		};
	fileLoader.AddCommand(bonusItemEndLevelCommand);

	return fileLoader.LoadFile(levelPath);
}

PacmanLevel::Tile* PacmanLevel::GetTileForSymbol(char symbol)
{
	for(size_t i = 0; i < mTiles.size(); i++)
	{
		if(mTiles[i].symbol == symbol)
		{
			return &mTiles[i];
		}
	}

	return nullptr;
}