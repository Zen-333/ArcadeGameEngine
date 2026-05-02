#include "ArcadeScene.h"
#include "Line2D.h"
#include "Vec2D.h"
#include "Screen.h"
#include "Color.h"
#include "GameController.h"
#include <iostream>
#include "App.h"
#include "AARectangle.h"

ArcadeScene::ArcadeScene()
{
}

void ArcadeScene::Init()
{
	//mTempSS.Load("ArcadeFont");

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

	MouseButtonAction mouseAction;
	mouseAction.mouseButton = GameController::LeftMouseButton();
	mouseAction.mouseInputAction = [](InputState state, const MousePosition& position)
	{
			if(GameController::IsPressed(state))
			{
				std::cout << "Left Mouse button pressed! " << std::endl;
			}
	};

	mGameController.AddMouseButtonAction(mouseAction);

	mGameController.SetMouseMovedAction([](const MousePosition& mousePosition)
	{
			//std::cout << "Mouse position x: " << mousePosition.xPos << " y: " << mousePosition.yPos << std::endl;

	});

}

void ArcadeScene::Update(uint32_t dt)
{
}

void ArcadeScene::Draw(Screen& theScreen)
{
	//Line2D line = { Vec2D(0,0), Vec2D(100, 100) };

	//theScreen.Draw(line, Color::Cyan());

	//theScreen.Draw(mTempSS, "z", Vec2D::Zero);

	const BitmapFont& font = App::Singleton().GetFont();

	AARectangle rect = { Vec2D::Zero, App::Singleton().Width(), App::Singleton().Height() };

	Vec2D textDrawPosition;
	textDrawPosition = font.GetDrawPosition(GetSceneName(), rect, BFXA_CENTER, BFYA_CENTER);

	theScreen.Draw(font, GetSceneName(), textDrawPosition, Color::Red());
}

const std::string& ArcadeScene::GetSceneName() const
{
	static std::string sceneName = "Arcade";
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
