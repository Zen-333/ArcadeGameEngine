#include "ArcadeScene.h"
#include "Line2D.h"
#include "Vec2D.h"
#include "Screen.h"
#include "Color.h"
#include "GameController.h"
#include <iostream>

ArcadeScene::ArcadeScene()
{
}

void ArcadeScene::Init()
{
	ButtonAction action;
	action.Key = GameController::ActionKey();
	action.action = [](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			std::cout << "Action button pressed!" << std::endl;
		}
	};

	mGameController.AddInputActionForKey(action);
}

void ArcadeScene::Update(uint32_t dt)
{
}

void ArcadeScene::Draw(Screen& theScreen)
{
	Line2D line = { Vec2D(0,0), Vec2D(100, 100) };

	theScreen.Draw(line, Color::Cyan());
}

const std::string& ArcadeScene::GetSceneName() const
{
	static std::string sceneName = "Arcade Scene";
	return sceneName;
}

std::unique_ptr<Scene> ArcadeScene::GetScene(eGame game)
{
	switch(game)
	{
		case TETRIS:
		{

		}
		break;

		case BREAK_OUT:
		{

		}
		break;

		case ASTEROIDS:
		{

		}
		break;

		case PACMAN:
		{

		}
		break;

		default:
		{

		}
		break;

	}

	return nullptr;
}
