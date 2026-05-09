#include "ButtonOptionsScene.h"
#include "App.h"
#include "BitmapFont.h"
#include "Utils.h"

ButtonOptionsScene::ButtonOptionsScene(const std::vector<std::string>& optionNames, const Color& textColor):
	mHighlightedOption(0)
{
	const BitmapFont& font = App::Singleton().GetFont();

	for (size_t i = 0; optionNames.size(); i++) {
		mButtons.push_back(Button(font, textColor));
		mButtons.back().SetButtonText(optionNames[i]);
	}

	if (optionNames.size() > 0) {
		mButtons[mHighlightedOption].SetHighlighted(true);
	}
}

void ButtonOptionsScene::Init()
{
	ButtonAction upAction;
	upAction.Key = GameController::UpKey();
	upAction.action = [this](uint32_t dt, InputState state) {
			if (GameController::IsPressed(state)) {
				SetPreviousButtonHighlighted();
			}
		};
	mGameController.AddInputActionForKey(upAction);

	ButtonAction downAction;
	downAction.Key = GameController::DownKey();
	downAction.action = [this](uint32_t dt, InputState state) {
			if (GameController::IsPressed(state)) {
				SetNextButtonHighlighted();
			}
		};
	mGameController.AddInputActionForKey(downAction);

	ButtonAction acceptAction;
	acceptAction.Key = GameController::ActionKey();
	acceptAction.action = [this](uint32_t dt, InputState state) {
			if (GameController::IsPressed(state)) {
				ExecuteCurrentButtonAction();
			}
		};
	mGameController.AddInputActionForKey(acceptAction);


}

void ButtonOptionsScene::Update(uint32_t dt)
{
}

void ButtonOptionsScene::Draw(Screen& theScreen)
{
	for (auto& button : mButtons) {
		button.Draw(theScreen);
	}
}

void ButtonOptionsScene::SetButtonActions(const std::vector<Button::ButtonAction>& buttonActions)
{
	for (size_t i = 0; i < mButtons.size(); i++) {
		mButtons[i].SetButtonAction(buttonActions[i]);
	}
}

void ButtonOptionsScene::SetNextButtonHighlighted()
{
	mHighlightedOption = (mHighlightedOption + 1) % mButtons.size();
	HighlightCurrentButton();
}

void ButtonOptionsScene::SetPreviousButtonHighlighted()
{
	--mHighlightedOption;

	if (mHighlightedOption < 0) {
		mHighlightedOption = static_cast<int>(mButtons.size()) - 1;
	}

	HighlightCurrentButton();
}

void ButtonOptionsScene::ExecuteCurrentButtonAction()
{
	mButtons[mHighlightedOption].ExecuteAction();
}

void ButtonOptionsScene::HighlightCurrentButton()
{
	for (auto& button : mButtons) {
		button.SetHighlighted(false);
	}

	mButtons[mHighlightedOption].SetHighlighted(true);
}
