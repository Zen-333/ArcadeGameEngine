
#include "ScreenBuffer.h"
#include "Color.h"
#include <SDL.h>
#include <cassert>


ScreenBuffer::ScreenBuffer() : mSurface(nullptr)
{

}

ScreenBuffer::ScreenBuffer(const ScreenBuffer& screenBuffer)
{
	mSurface = SDL_CreateRGBSurfaceWithFormat(0, screenBuffer.mSurface->w, screenBuffer.mSurface->h, 0, screenBuffer.mSurface->format->format);

	SDL_BlitSurface(screenBuffer.mSurface, nullptr, mSurface, nullptr); // copy all the pixels from one surface to another

}

ScreenBuffer::~ScreenBuffer()
{
	if(mSurface)
	{
		SDL_FreeSurface(mSurface); // Frees all memory associated with the surface
	}
}

ScreenBuffer& ScreenBuffer::operator=(const ScreenBuffer& screenBuffer)
{
	if (this == &screenBuffer) return *this;

	if(mSurface != nullptr)
	{
		SDL_FreeSurface(mSurface);
		mSurface = nullptr;
	}

	if(screenBuffer.mSurface != nullptr)
	{
		mSurface = SDL_CreateRGBSurfaceWithFormat(0, screenBuffer.mSurface->w, screenBuffer.mSurface->h, 0, screenBuffer.mSurface->format->format);

		SDL_BlitSurface(screenBuffer.mSurface, nullptr, mSurface, nullptr); // copy all the pixels from one surface to another

	}

	return *this;
}

void ScreenBuffer::Init(uint32_t format, uint32_t width, uint32_t height)
{
	mSurface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 0, format);
	Clear();
}

void ScreenBuffer::Clear(const Color& c)
{
	assert(mSurface);

	if(mSurface)
	{
		SDL_FillRect(mSurface, nullptr, c.GetPixelColor());
	}
}

void ScreenBuffer::SetPixel(const Color& color, int x, int y)
{
	assert(mSurface);
	if(mSurface && (y < mSurface->h && y >= 0 && x >= 0 && x < mSurface->w))
	{
		SDL_LockSurface(mSurface); // Locking makes sure you can safely write to the raw pixel array in RAM

		uint32_t* pixels = (uint32_t*)mSurface->pixels; // 1D array of all the pixels

		size_t index = GetIndex(y, x);
		Color surfaceColor = Color(pixels[index]); // destinationColor
		pixels[index] = Color::Evaluate1MinueSourceAlpha(color, surfaceColor).GetPixelColor();



		SDL_UnlockSurface(mSurface); // Unlocks the surface so SDL (or the GPU) can use it again
	}

}

uint32_t ScreenBuffer::GetIndex(int r, int c)
{
	assert(mSurface);
	if(mSurface)
	{
		return r * mSurface->w + c;
	}
	return 0;

	//Row 0: [0] [1] [2] [3]
	//Row 1 : [4] [5] [6] [7]
	//Row 2 : [8] [9] [10] [11]
}
