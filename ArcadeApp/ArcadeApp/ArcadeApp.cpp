#include <iostream>
#include "Color.h"
#include "SDL.h"
#include "Screen.h"
#include "Line2D.h"
#include "Star2D.h"
#include "Triangle.h"

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

	//Line2D line = { Vec2D(0,0), Vec2D(SCREEN_WIDTH/2, SCREEN_HEIGHT - 50) };

	Star2D star(100.0f, 65.0f, MiddleScreen, 5);
	Vec2D point1 = MiddleScreen - Vec2D(30, -30);
	Vec2D point2 = MiddleScreen + Vec2D(30, 30);

	Triangle triangle(MiddleScreen, point1, point2);


	//theScreen.Draw(star, Color::Green());

	//theScreen.Draw(line, Color::Cyan());
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
		
		
		//star.RotateStar(rotationSpeed * deltaTime, MiddleScreen);
		theScreen.Draw(triangle, Color::Green());

		//theScreen.Draw(line, Color::Cyan());
		//theScreen.Draw(line, Color::Cyan());
		theScreen.SwapScreen();

		cout << deltaTime << endl;

		// TESTING BRANCH RECTANGLE //
	}

	return 0;
}

