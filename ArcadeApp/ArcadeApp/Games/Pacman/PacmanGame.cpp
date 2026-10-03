#include "Pacman/PacmanGame.h"
#include "App.h"
#include "PacmanGameUtils.h"

namespace {
	const std::string SCORE_STR = "Score ";
	const std::string PACMAN_LIFE_SPRITE_NAME = "pac_man_left_idle";
	const size_t MAX_NUM_LIVES = 3;
}

void PacmanGame::Init(GameController& controller)
{
	mPacmanSpriteSheet.Load("PacmanSprites");
	mPacman.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Pacman_animations.txt", Vec2D::Zero, PACMAN_MOVEMENT_SPEED, false);

	mLevel.Init(App::Singleton().GetBasePath() + "Assets/Pacman_level.txt",&mPacmanSpriteSheet);

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
}

void PacmanGame::Update(uint32_t dt)
{
	UpdatePacmanMovement();
	mPacman.Update(dt);

	for(size_t i = 0; i < NUM_GHOSTS; i++)
	{
		mGhosts[i].Update(dt);
	}

	mLevel.Update(dt,mPacman, mGhosts);

	if(mLevel.IsLevelOver())
	{
		mLevel.IncreaseLevel();
		ResetLevel();
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

	{
		Vec2D levelOffset = mLevel.GetLayoutOffset();

		AARectangle highScoreRect = AARectangle(Vec2D(0, 4), App::Singleton().Width(), levelOffset.GetY());

		const auto& font = App::Singleton().GetFont();
		Vec2D textDrawPosition;

		std::string scoreStr = std::to_string(mPacman.Score());

		textDrawPosition = font.GetDrawPosition(SCORE_STR + scoreStr, highScoreRect, BFXA_CENTER, BFYA_CENTER);

		screen.Draw(font, SCORE_STR + scoreStr, textDrawPosition);
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
	mPacman.MoveTo(mLevel.GetPacmanSpawnLocation());
	mPacman.ResetToFirstAnimation();
}

void PacmanGame::SetupGhosts()
{
	mGhosts.resize(NUM_GHOSTS);

	Ghost blinky;
	blinky.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Ghost_animations.txt", mLevel.GetGhostSpawnPoints()[BLINKY], GHOST_MOVEMENT_SPEED, true, Color::Red());
	blinky.SetMovementDirection(PACMAN_MOVEMENT_LEFT);
	mGhosts[BLINKY] = blinky;

	Ghost pinky;
	pinky.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Ghost_animations.txt", mLevel.GetGhostSpawnPoints()[PINKY], GHOST_MOVEMENT_SPEED, true, Color::Pink());
	pinky.SetMovementDirection(PACMAN_MOVEMENT_DOWN);
	mGhosts[PINKY] = pinky;

	Ghost inky;
	inky.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Ghost_animations.txt", mLevel.GetGhostSpawnPoints()[INKY], GHOST_MOVEMENT_SPEED, true, Color::Cyan());
	inky.SetMovementDirection(PACMAN_MOVEMENT_UP);
	mGhosts[INKY] = inky;

	Ghost clyde;
	clyde.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Ghost_animations.txt", mLevel.GetGhostSpawnPoints()[CLYDE], GHOST_MOVEMENT_SPEED, true, Color::Orange());
	clyde.SetMovementDirection(PACMAN_MOVEMENT_UP);
	mGhosts[CLYDE] = clyde;
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

