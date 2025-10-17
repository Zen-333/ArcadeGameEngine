#include <iostream>
#include "App.h"
#include <SDL.h>
#include "ArcadeScene.h"
#include <cassert>

App& App::Singleton()
{
	static App theApp;
	return theApp;
}

bool App::Init(uint32_t width, uint32_t height, uint32_t mag)
{
	mnoptrWindow = mScreen.Init(width, height, mag);

	std::unique_ptr<ArcadeScene> arcadeScene = std::make_unique<ArcadeScene>();

	PushScene(std::move(arcadeScene));

	return mnoptrWindow != nullptr;
}

void App::Run()
{

	if(mnoptrWindow)
	{

		//SDL_Event sdlEvent;
		bool running = true;

		uint32_t lastTick = SDL_GetTicks(); // return milliseconds since SDL init
		uint32_t currentTick = lastTick;

		uint32_t dt = 10; // update the app rate 10 milliseconds 10ms means 100 updates per second (dt = delta time)
		uint32_t accumulator = 0; // it stores leftover time between frames

		mInputController.Init([&running](uint32_t dt, InputState state) {
			
			running = false;
			
		});


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
			mInputController.Update(dt);

			//Update
			Scene* topScene = App::TopScene();
			assert(topScene && "why dont we have a scene?");
			if (topScene)
			{
				while (accumulator >= dt)
				{
					// update current scene by dt
					topScene->Update(dt);
					//std::cout << "Delta time step: " << accumulator << std::endl;
					accumulator -= dt;
				}
				topScene->Draw(mScreen);
			}

			//Render & Draw

			mScreen.SwapScreen();

		}
	}

}

void App::PushScene(std::unique_ptr<Scene> scene)
{
	assert(scene && "dont push nullptr");
	if(scene)
	{
		scene->Init();
		mInputController.SetGameController(scene->GetGameController());
		mSceneStack.emplace_back(std::move(scene));
		SDL_SetWindowTitle(mnoptrWindow, TopScene()->GetSceneName().c_str());
	}
}

void App::PopScene()
{
	if (mSceneStack.size() > 1)
	{
		mSceneStack.pop_back();
	}

	if(TopScene())
	{
		mInputController.SetGameController(TopScene()->GetGameController());
		SDL_SetWindowTitle(mnoptrWindow, TopScene()->GetSceneName().c_str());

	}
}

Scene* App::TopScene()
{
	if(mSceneStack.empty())
	{
		return nullptr;
	}

	return mSceneStack.back().get();
}
