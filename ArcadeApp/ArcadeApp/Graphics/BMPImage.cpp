#include "BMPImage.h"
#include <SDL.h>

BMPImage::BMPImage(): mWidth(0), mHeigth(0)
{

}

bool BMPImage::Load(const std::string& path)
{
	SDL_Surface* bmpSurface = SDL_LoadBMP(path.c_str());

	if(bmpSurface == nullptr)
	{
		return false;
	}

	mWidth = bmpSurface->w;
	mHeigth = bmpSurface->h;

	uint32_t lengthOfFile = mWidth * mHeigth;

	mPixels.reserve(lengthOfFile); // tells the vector how much memory to occupy

	SDL_LockSurface(bmpSurface); // locks the surface so that it doesnt change while we use it

	// bmpSurface->pixels is a memory address (a pointer). 
	// It points to the very first pixel of the image. However, in C++, 
	// if you have a pointer to a memory address, you can use the [] (subscript) operator to look at the neighbors of that address.
	uint32_t* pixels = static_cast<uint32_t*>(bmpSurface->pixels);

	for(uint32_t i = 0; i < lengthOfFile; i++)
	{
		mPixels.push_back(Color(pixels[i]));
	}

	SDL_UnlockSurface(bmpSurface);
	SDL_FreeSurface(bmpSurface); // we delete it to prevent memory leak

	return true;
}
