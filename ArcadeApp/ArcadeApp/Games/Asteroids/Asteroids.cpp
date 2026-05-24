#include "Asteroids.h"

Asteroids::Asteroids()
{
}

void Asteroids::Init(GameController& controller)
{
	mSpaceShip.Init();
}

void Asteroids::Update(uint32_t dt)
{
	mSpaceShip.Update(dt);
}

void Asteroids::Draw(Screen& screen)
{
	mSpaceShip.Draw(screen);
}

const std::string& Asteroids::GetName() const
{
	static std::string name = "Asteroids <>";
	return name;
}
