#pragma once

#include "Screen.h"
#include <stdint.h>
#include <vector>
#include <memory>
#include "Scene.h"

struct SDL_Window;


class App
{
public:
	static App& Singleton(); // one object of this type
	bool Init(uint32_t width, uint32_t height, uint32_t mag);
	void Run();

	inline uint32_t Width() const { return mScreen.Width(); }
	inline uint32_t Height() const { return mScreen.Height(); }

	void PushScene(std::unique_ptr<Scene> scene);
	void PopScene();
	Scene* TopScene();

private:
	Screen mScreen;
	SDL_Window* mnoptrWindow;

	std::vector<std::unique_ptr<Scene>> mSceneStack;
};