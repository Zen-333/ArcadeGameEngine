#include <iostream>
#include "App.h"
#include <SDL.h>
#include "Color.h"

App& App::Singleton()
{
	static App theApp;
	return theApp;
}

bool App::Init(uint32_t width, uint32_t height, uint32_t mag)
{
	mnoptrWindow = mScreen.Init(width, height, mag);
	return mnoptrWindow != nullptr;
}

void App::Run()
{

	if(mnoptrWindow)
	{

		SDL_Event sdlEvent;
		bool running = true;

		uint32_t lastTick = SDL_GetTicks(); // return milliseconds since SDL init
		uint32_t currentTick = lastTick;

		uint32_t dt = 10; // update the app rate 10 milliseconds 10ms means 100 updates per second (dt = delta time)
		uint32_t accumulator = 0; // it stores leftover time between frames

		while (running)
		{
			currentTick = SDL_GetTicks();
			uint32_t frameTime = currentTick - lastTick; // times elapsed since the last iteration

			if(frameTime > 300)
			{
				frameTime = 300;
			}

			// Capping frameTime to 300 prevents a huge time jump if the game was paused, suspended, or debugging paused — avoids trying to run thousands of updates to "catch up" (the “spiral of death”).

			lastTick = currentTick;
			accumulator += frameTime;

			// Input
			while (SDL_PollEvent(&sdlEvent))
			{
				switch (sdlEvent.type)
				{
				case SDL_QUIT:
					running = false;
					break;
				}
			}

			//Update
			while(accumulator >= dt)
			{
				// update current scene by dt
				std::cout << "Delta time step: " << accumulator << std::endl;
				accumulator -= dt;
			}

			//Render & Draw
			mScreen.SwapScreen();

		}
	}

}
