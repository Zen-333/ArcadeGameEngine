#include "Pacman/PacmanGame.h"
#include "App.h"
#include "PacmanGameUtils.h"

namespace {
	const std::string SCORE_STR = "Score ";
	const std::string PACMAN_LIFE_SPRITE_NAME = "pac_man_left_idle";
	const size_t MAX_NUM_LIVES = 3;
	const uint32_t RELEASE_GHOST_TIME = 5000;
	const std::string READY_STR = "Ready!";
	const std::string GAME_OVER_STR = "Game Over";
	const uint32_t LEVEL_STARTING_TIME = 3000;
}

void PacmanGame::Init(GameController& controller)
{
	mReleaseGhostTimer = 0;
	mPacmanSpriteSheet.Load("PacmanSprites");
	mPacman.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Pacman_animations.txt", Vec2D::Zero, PACMAN_MOVEMENT_SPEED, false);

	mLevel.Init(App::Singleton().GetBasePath() + "Assets/Pacman_level.txt",&mPacmanSpriteSheet);

	mStringRect = AARectangle(Vec2D(0, mLevel.GetInGameTextYPos()), App::Singleton().Width(), mPacman.GetBoundingBox().GetHeight());


	SetupGhosts();
	ResetGame();

	ButtonAction leftAction;
	leftAction.Key = GameController::LeftKey();
	leftAction.action = [this](uint32_t dt, InputState state)
	{
			HandleGameControllerState(dt, state, PACMAN_MOVEMENT_LEFT);
	};
	controller.AddInputActionForKey(leftAction);

	ButtonAction rightAction;
	rightAction.Key = GameController::RightKey();
	rightAction.action = [this](uint32_t dt, InputState state)
		{
			HandleGameControllerState(dt, state, PACMAN_MOVEMENT_RIGHT);
		};
	controller.AddInputActionForKey(rightAction);

	ButtonAction upAction;
	upAction.Key = GameController::UpKey();
	upAction.action = [this](uint32_t dt, InputState state)
		{
			HandleGameControllerState(dt, state, PACMAN_MOVEMENT_UP);
		};
	controller.AddInputActionForKey(upAction);

	ButtonAction downAction;
	downAction.Key = GameController::DownKey();
	downAction.action = [this](uint32_t dt, InputState state)
		{
			HandleGameControllerState(dt, state, PACMAN_MOVEMENT_DOWN);
		};
	controller.AddInputActionForKey(downAction);

	ButtonAction backAction;
	backAction.Key = GameController::CancelKey();
	backAction.action = [this](uint32_t dt, InputState state) {

		if (mGameState == GAME_OVER && GameController::IsPressed(state)) {
			App::Singleton().PopScene();
		}
		};

	controller.AddInputActionForKey(backAction);
}

void PacmanGame::Update(uint32_t dt)
{
	if(mGameState == LEVEL_STARTING)
	{
		mPacman.ResetGhostEatenMultiplier();
		mLevelStartingTimer += dt;

		if(mLevelStartingTimer >= LEVEL_STARTING_TIME)
		{
			mLevelStartingTimer = 0;
			mGameState = PLAY_GAME;
		}
	}else if(mGameState == PLAY_GAME)
	{
		UpdatePacmanMovement();
		mPacman.Update(dt);

		mReleaseGhostTimer += dt;

		for(size_t i = 0; i < NUM_GHOSTS; i++)
		{
			Ghost& ghost = mGhosts[i];
			GhostAI& ghostAI = mGhostsAIs[i];

			if(mReleaseGhostTimer >= RELEASE_GHOST_TIME && ghostAI.IsInPen() && !ghost.IsReleased())
			{
				mReleaseGhostTimer = 0;
				ghost.ReleaseFromPen();
			}

			auto direction = ghostAI.Update(dt, mPacman, mLevel, mGhosts);

			if((ghost.IsReleased() && ghost.CanChangeDirection()) || (ghost.IsReleased() && ghostAI.WantsToLeavePen() && direction != PACMAN_MOVEMENT_NONE))
			{
				if(direction != ghost.GetMovementDirection())
				{
					ghost.SetMovementDirection(direction);
					ghost.LockCanChangeDirection();
				}
			}

			ghost.Update(dt);

			if(ghost.IsVulnerable() && mPacman.GetEactingBoundingBox().Intersects(ghost.GetBoundingBox()))
			{
				ghost.EatenByPacman();
				mPacman.AteGhost(ghost.GetPoints());
			}else if(ghost.IsAlive() && ghost.GetEactingBoundingBox().Intersects(mPacman.GetBoundingBox()))
			{
				mNumLives--;
				mGameState = LOST_LIFE;
				mPacman.EatenByGhost();
				mPressedDirection = PACMAN_MOVEMENT_NONE;
				mPacman.SetMovementDirection(PACMAN_MOVEMENT_NONE);
				return;
			}
		}

		mLevel.Update(dt,mPacman, mGhosts, mGhostsAIs);

		if(mLevel.IsLevelOver())
		{
			mLevel.IncreaseLevel();
			ResetLevel();
		}
	}else if(mGameState == LOST_LIFE)
	{
		mPacman.Update(dt);
		if(mPacman.IsFinishedAnimation())
		{
			if(mNumLives >= 0)
			{
				ResetLevel();
			}else
			{
				mGameState = GAME_OVER;
			}
		}
	}

}

void PacmanGame::Draw(Screen& screen)
{
	mLevel.Draw(screen);
	mPacman.Draw(screen);

	for (auto& ghost: mGhosts)
	{
		ghost.Draw(screen);
	}

	const auto& font = App::Singleton().GetFont();
	Vec2D textDrawPosition;
	{
		Vec2D levelOffset = mLevel.GetLayoutOffset();

		AARectangle highScoreRect = AARectangle(Vec2D(0, 4), App::Singleton().Width(), levelOffset.GetY());

		std::string scoreStr = std::to_string(mPacman.Score());

		textDrawPosition = font.GetDrawPosition(SCORE_STR + scoreStr, highScoreRect, BFXA_CENTER, BFYA_CENTER);

		screen.Draw(font, SCORE_STR + scoreStr, textDrawPosition);
	}

	if(mGameState == LEVEL_STARTING)
	{
		textDrawPosition = font.GetDrawPosition(READY_STR, mStringRect, BFXA_CENTER, BFYA_CENTER);
		screen.Draw(font, READY_STR, textDrawPosition, Color::Yellow());
	}else if(mGameState == GAME_OVER)
	{
		textDrawPosition = font.GetDrawPosition(GAME_OVER_STR, mStringRect, BFXA_CENTER, BFYA_CENTER);
		screen.Draw(font, GAME_OVER_STR, textDrawPosition, Color::Yellow());
	}

	DrawLives(screen);
}

void PacmanGame::DrawLives(Screen& screen)
{
	const uint32_t X_PAD = 1;

	auto sprite = mPacmanSpriteSheet.GetSprite(PACMAN_LIFE_SPRITE_NAME);

	uint32_t xPos = X_PAD;

	for(int i = 0; i < mNumLives; i++)
	{
		screen.Draw(mPacmanSpriteSheet, PACMAN_LIFE_SPRITE_NAME, Vec2D(xPos, App::Singleton().Height() - sprite.height));
		xPos += X_PAD + sprite.width;
	}
}

const std::string& PacmanGame::GetName() const
{
	static std::string name = "!Pacman Game!";
	return name;
}

void PacmanGame::ResetGame()
{
	mNumLives = MAX_NUM_LIVES;
	mPressedDirection = PACMAN_MOVEMENT_NONE;
	mPacman.ResetScore();
	mLevel.ResetToFirstLevel();
	ResetLevel();
}

void PacmanGame::ResetLevel()
{
	mReleaseGhostTimer = 0;
	mGameState = LEVEL_STARTING;
	mPacman.MoveTo(mLevel.GetPacmanSpawnLocation());
	mPacman.ResetToFirstAnimation();

	for(auto& ghost : mGhosts)
	{
		ghost.ResetToFirstPosition();
	}

	mGhosts[BLINKY].ReleaseFromPen();
}

void PacmanGame::SetupGhosts()
{
	const Vec2D BLINKY_SCATTER_POS = Vec2D(App::Singleton().Width() - 24, 0);
	const Vec2D INKY_SCATTER_POS = Vec2D(App::Singleton().Width(), App::Singleton().Height());
	const Vec2D PINKY_SCATTER_POS = Vec2D(24, 0);
	const Vec2D CLYDE_SCATTER_POS = Vec2D(0, App::Singleton().Height());

	mGhosts.resize(NUM_GHOSTS);
	mGhostsAIs.resize(NUM_GHOSTS);

	Ghost blinky;
	blinky.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Ghost_animations.txt", mLevel.GetGhostSpawnPoints()[BLINKY], GHOST_MOVEMENT_SPEED, true, Color::Red());
	blinky.SetMovementDirection(PACMAN_MOVEMENT_LEFT);
	mGhosts[BLINKY] = blinky;

	auto blinkyAI = GhostAI();
	blinkyAI.Init(mGhosts[BLINKY],blinky.GetBoundingBox().GetWidth(), BLINKY_SCATTER_POS,mLevel.GetGhostSpawnPoints()[PINKY],mLevel.GetGhostSpawnPoints()[BLINKY], BLINKY);

	mGhostsAIs[BLINKY] = blinkyAI;

	Ghost pinky;
	pinky.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Ghost_animations.txt", mLevel.GetGhostSpawnPoints()[PINKY], GHOST_MOVEMENT_SPEED, true, Color::Pink());
	pinky.SetMovementDirection(PACMAN_MOVEMENT_DOWN);
	mGhosts[PINKY] = pinky;

	auto pinkyAI = GhostAI();
	pinkyAI.Init(mGhosts[PINKY], pinky.GetBoundingBox().GetWidth(), PINKY_SCATTER_POS, mLevel.GetGhostSpawnPoints()[PINKY], mLevel.GetGhostSpawnPoints()[BLINKY], PINKY);

	mGhostsAIs[PINKY] = pinkyAI;

	Ghost inky;
	inky.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Ghost_animations.txt", mLevel.GetGhostSpawnPoints()[INKY], GHOST_MOVEMENT_SPEED, true, Color::Cyan());
	inky.SetMovementDirection(PACMAN_MOVEMENT_UP);
	mGhosts[INKY] = inky;

	auto inkyAI = GhostAI();
	inkyAI.Init(mGhosts[INKY], inky.GetBoundingBox().GetWidth(), INKY_SCATTER_POS, mLevel.GetGhostSpawnPoints()[PINKY], mLevel.GetGhostSpawnPoints()[BLINKY], INKY);

	mGhostsAIs[INKY] = inkyAI;

	Ghost clyde;
	clyde.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Ghost_animations.txt", mLevel.GetGhostSpawnPoints()[CLYDE], GHOST_MOVEMENT_SPEED, true, Color::Orange());
	clyde.SetMovementDirection(PACMAN_MOVEMENT_UP);
	mGhosts[CLYDE] = clyde;

	auto clydeAI = GhostAI();
	clydeAI.Init(mGhosts[CLYDE], clyde.GetBoundingBox().GetWidth(), CLYDE_SCATTER_POS, mLevel.GetGhostSpawnPoints()[PINKY], mLevel.GetGhostSpawnPoints()[BLINKY], CLYDE);

	mGhostsAIs[CLYDE] = clydeAI;

	for(size_t i = 0; i < NUM_GHOSTS; i++)
	{
		mGhosts[i].SetGhostDelegate(mGhostsAIs[i]);
	}
}

void PacmanGame::UpdatePacmanMovement()
{
	if(mPressedDirection != PACMAN_MOVEMENT_NONE)
	{
		if(!mLevel.WillCollide(mPacman.GetBoundingBox(), mPressedDirection))
		{
			mPacman.SetMovementDirection(mPressedDirection);
		}
	}
}

void PacmanGame::HandleGameControllerState(uint32_t dt, InputState state, PacmanMovement direction)
{
	if(GameController::IsPressed(state))
	{
		mPressedDirection = direction;
	}else if(GameController::IsReleased(state) && mPressedDirection == direction)
	{
		mPressedDirection = PACMAN_MOVEMENT_NONE;
	}
}

