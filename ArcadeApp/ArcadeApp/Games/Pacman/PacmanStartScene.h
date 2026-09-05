#pragma once
#include "ButtonOptionsScene.h"

class PacmanStartScene: public ButtonOptionsScene {

public:

	PacmanStartScene();
	virtual void Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& theScreen) override;
	const std::string& GetSceneName() const override;

private:
};