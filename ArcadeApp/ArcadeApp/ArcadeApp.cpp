#include <iostream>
#include "Color.h"
#include "SDL.h"
#include "Screen.h"
#include "Line2D.h"
#include "Star2D.h"
#include "Triangle.h"
#include "AARectangle.h"
#include "Circle.h"

using namespace std;

const int SCREEN_WIDTH = 224;
const int SCREEN_HEIGHT = 288;
const int MAGNIFICATION = 3;

int main(int argc, char * argv[])
{

	// TESTING BRANCH MAIN //

	Screen theScreen;
	theScreen.Init(SCREEN_WIDTH, SCREEN_HEIGHT, MAGNIFICATION);

	Vec2D MiddleScreen(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);



	AARectangle rect = {Vec2D(SCREEN_WIDTH/ 2 - 25, SCREEN_HEIGHT/2 - 25), 50, 50};
	Star2D star(40, 20, MiddleScreen, 4);
	Circle circle = {Vec2D(SCREEN_WIDTH/2 + 50, SCREEN_HEIGHT/2 + 50), 50};

	theScreen.SwapScreen();

	SDL_Event sdlEvent;
	bool running = true;

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;

	float rotationSpeed = 6.283 / 5;

	while (running)
	{
		while(SDL_PollEvent(&sdlEvent))
		{
			switch(sdlEvent.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			}
		}

		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());

		//line.Rotate(rotationSpeed * deltaTime, MiddleScreen);
		theScreen.Draw(rect, Color::Blue(), true, Color::Blue());
		theScreen.Draw(circle, Color(0, 255, 0, 150), true, Color(0, 255, 0, 150));
		//theScreen.Draw(star, Color(0, 255, 0, 150), true, Color(0, 255, 0, 150));
		//theScreen.Draw(triangle, Color::Green(), true, Color::Red());
		theScreen.SwapScreen();

		cout << deltaTime << endl;

		// TESTING BRANCH RECTANGLE //
	}

	return 0;
}

