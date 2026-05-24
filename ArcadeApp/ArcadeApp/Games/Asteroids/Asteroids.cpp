#include "Asteroids.h"
#include "App.h"

Asteroids::Asteroids(): mSpaceShip(Vec2D(App::Singleton().Width() / 2.0f, App::Singleton().Height() / 2.0f))
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
