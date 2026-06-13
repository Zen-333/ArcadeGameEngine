#include "Screen.h"
#include "Vec2D.h"
#include <SDL.h>
#include <cassert>
#include "Line2D.h"
#include "Star2D.h"
#include "Triangle.h"
#include "AARectangle.h"
#include "Circle.h"
#include "Utils.h"
#include <cmath> 
#include <algorithm> // to use std::sort 
#include "BMPImage.h"
#include "SpriteSheet.h"
#include "BitmapFont.h"


Screen::Screen() : mWidth(0), mHeight(0), moptrWindow(nullptr), mnoptrWindowSurface(nullptr), mRenderer(nullptr), mPixelFormat(nullptr), mTexture(nullptr)
{

}

Screen::~Screen() 
{
	if(mPixelFormat)
	{
		SDL_FreeFormat(mPixelFormat);
		mPixelFormat = nullptr;
	}
	if (mTexture) {
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
	}
	if (mRenderer) {
		SDL_DestroyRenderer(mRenderer);
		mRenderer = nullptr;
	}


	

	if(moptrWindow)
	{
		SDL_DestroyWindow(moptrWindow);
		moptrWindow = nullptr;
	}

	SDL_Quit();
}

SDL_Window* Screen::Init(uint32_t w, uint32_t h, uint32_t mag, bool fast)
{

	mFast = fast;

	if (SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << "Error SDL_Init Failed: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	mWidth = w;
	mHeight = h;

	moptrWindow = SDL_CreateWindow("Arcade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth * mag, mHeight * mag, 0);

	if (moptrWindow)
	{
		uint8_t rClear = 0;
		uint8_t gClear = 0;
		uint8_t bClear = 0;
		uint8_t aClear = 255;

		if(mFast)
		{
			mRenderer = SDL_CreateRenderer(moptrWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			if(mRenderer == nullptr)
			{
				std::cout << "SDL_CreateRenderer failed" << std::endl;
				return nullptr;
			}

			SDL_SetRenderDrawColor(mRenderer, rClear, gClear, bClear, aClear);
		}
		else {
			mnoptrWindowSurface = SDL_GetWindowSurface(moptrWindow); // front buffer
		}

		mPixelFormat = SDL_AllocFormat(SDL_GetWindowPixelFormat(moptrWindow));

		if(mFast)
		{
			mTexture = SDL_CreateTexture(mRenderer, mPixelFormat->format, SDL_TEXTUREACCESS_STREAMING, w, h);
		}

		//SDL_PixelFormat* pixelFormat = mnoptrWindowSurface->format; // That surface usually uses the native pixel format of your display (often RGB888 or BGR888, i.e. no alpha channel).

		//SDL_PixelFormat* pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888); // to allow alpha. This explicitly tells SDL: “use a pixel format that has 32 bits per pixel with alpha.”

		Color::InitColorFormat(mPixelFormat);

		mClearColor = Color(rClear, gClear, bClear, aClear);

		mBackBuffer.Init(mPixelFormat->format, mWidth, mHeight);
		mBackBuffer.Clear(mClearColor);
	}

	return moptrWindow;


}

void Screen::SwapScreen() 
{
	assert(moptrWindow);

	if(moptrWindow)
	{
		ClearScreen();
			
		if(mFast)
		{
			uint8_t* textureData = nullptr;
			int texturePitch = 0;
			if (SDL_LockTexture(mTexture, nullptr, (void**)&textureData, &texturePitch) >= 0) {
				SDL_Surface* surface = mBackBuffer.GetSurface();
				memcpy(textureData, surface->pixels, surface->w * surface->h * mPixelFormat->BytesPerPixel);

				SDL_UnlockTexture(mTexture); // exception error here

				SDL_RenderCopy(mRenderer, mTexture, nullptr, nullptr);

				SDL_RenderPresent(mRenderer);
			}
		}
		else {

			SDL_BlitScaled(mBackBuffer.GetSurface(), nullptr, mnoptrWindowSurface, nullptr);

			//Why “scaled” ?

				//If your window is bigger than your back buffer(because of mag), SDL will stretch it automatically.

				//Example : back buffer = 224x288, window = 672x864 ? each pixel is 3×3 on the screen.

			SDL_UpdateWindowSurface(moptrWindow);

		}

		mBackBuffer.Clear(mClearColor);
	}

}

void Screen::Draw(int x, int y, const Color& color) 
{
	assert(moptrWindow);

	if (moptrWindow)
	{
		mBackBuffer.SetPixel(color, x, y);

	}
}

void Screen::Draw(const Vec2D& point, const Color& color) 
{
	assert(moptrWindow);

	if (moptrWindow)
	{
		mBackBuffer.SetPixel(color, point.GetX(), point.GetY());

	}
}

void Screen::Draw(const Line2D& line, const Color& color)
{
	assert(moptrWindow);

	if (moptrWindow)
	{
		int dx, dy;

		int x0 = roundf(line.GetP0().GetX());
		int y0 = roundf(line.GetP0().GetY());
		int x1 = roundf(line.GetP1().GetX());
		int y1 = roundf(line.GetP1().GetY());

		dx = x1 - x0;
		dy = y1 - y0;

		// x going left or right 1 or -1 same for y up or down
		signed const char ix((dx > 0) - (dx < 0)); // evaluate to 1 or -1
		signed const char iy((dy > 0) - (dy < 0)); // signed means it can hold positive and negative values

		dx = abs(dx) * 2; // abs it returns an absolute positive int even if negative it makes it positive
		dy = abs(dy) * 2;

		Draw(x0, y0, color);

		if(dx >= dy)
		{
			// go along in the x

			int d = dy - dx / 2; // dicision variable divide by 2 because we multiplied by 2 

			while(x0 != x1)
			{
				if(d >= 0)
				{
					d -= dx;
					y0 += iy;
				}
				d += dy;
				x0 += ix;

				Draw(x0, y0, color);
			}
		}
		else
		{
			// go along in the y

			int d = dx - dy / 2;

			while(y0 != y1)
			{
				if( d >= 0)
				{
					d -= dy;
					x0 += ix;
				}

				d += dx;
				y0 += iy;

				Draw(x0, y0, color);
			}
		}
	}
}

void Screen::Draw(const Star2D& star, const Color& color, bool fill, const Color& fillColor)
{
	if(fill)
	{
		FillPoly(star.GetPoints(), [fillColor](uint32_t x, uint32_t y) {return fillColor; });
	}

	for(int i = 0; i < star.mStarLines.size(); i++)
	{
		Draw(star.mStarLines[i], color);
	}

}

void Screen::Draw(const Triangle& triangle, const Color& color, bool fill, const Color& fillColor)
{
	if(fill)
	{
		FillPoly(triangle.GetPoints(), [fillColor](uint32_t x, uint32_t y) {return fillColor; });
	}

	Line2D line0(triangle.GetP0(), triangle.GetP1());
	Line2D line1(triangle.GetP1(), triangle.GetP2());
	Line2D line2(triangle.GetP2(), triangle.GetP0());

	Draw(line0, color);
	Draw(line1, color);
	Draw(line2, color);

}

void Screen::Draw(const AARectangle& rectangle, const Color& color, bool fill, const Color& fillColor)
{
	if (fill)
	{
		FillPoly(rectangle.GetPoints(), [fillColor](uint32_t x, uint32_t y) {return fillColor; });
	}

	const std::vector<Vec2D> points = rectangle.GetPoints();

	Line2D top(points[0], points[1]);
	Line2D left(points[1], points[2]);
	Line2D bottom(points[2], points[3]);
	Line2D right(points[3], points[0]);

	Draw(top, color);
	Draw(bottom, color);
	Draw(right, color);
	Draw(left, color);

}

void Screen::Draw(const Circle& circle, const Color& color, bool fill, const Color& fillColor)
{
	std::vector<Vec2D> circlePoints;
	std::vector<Line2D> lines;

	const float radian = 6.28319;
	const int sections = 30;
	const float angleSteps = (radian / sections);
	const float radius = circle.GetRadius();

	const float Cx = circle.GetCenterPoint().GetX() + radius;
	const float Cy = circle.GetCenterPoint().GetY();

	Vec2D p0(Cx, Cy);
	Vec2D p1 = p0;
	Line2D nextLineToDraw;

	for (int i = 0; i < sections; i++)
	{
		p1.Rotate(angleSteps, circle.GetCenterPoint());
		nextLineToDraw.SetP01(p1);
		nextLineToDraw.SetP0(p0);

		lines.push_back(nextLineToDraw);
		//Draw(nextLineToDraw, color);
		p0 = p1;
		circlePoints.push_back(p0);
	}

	if(fill)
	{
		FillPoly(circlePoints, [fillColor](uint32_t x, uint32_t y) {return fillColor; });
	}

	for(const Line2D& line : lines)
	{
		Draw(line, color);
	}

}

void Screen::Draw(const BMPImage& image, const Sprite& sprite, const Vec2D& pos, const Color& overlayColor, float rotation)
{
	float rVal = static_cast<float>(overlayColor.GetRed()) / 255.0f;
	float gVal = static_cast<float>(overlayColor.GetGreen()) / 255.0f;
	float bVal = static_cast<float>(overlayColor.GetBlue()) / 255.0f;
	float aVal = static_cast<float>(overlayColor.GetAlpha()) / 255.0f;

	uint32_t width = sprite.width;
	uint32_t height = sprite.height;

	const std::vector<Color>& pixels = image.GetPixels();

	auto topLeft = pos;
	auto topRight = pos + Vec2D(width, 0);
	auto bottomLeft = pos + Vec2D(0, height);
	auto bottomRight = pos + Vec2D(width, height);

	if(rotation != 0.0f)
	{
		Vec2D center = pos + Vec2D(width / 2.0f, height / 2.0f);
		topLeft.Rotate(rotation, center);
		topRight.Rotate(rotation, center);
		bottomLeft.Rotate(rotation, center);
		bottomRight.Rotate(rotation, center);
	}


	std::vector<Vec2D> points = { topLeft, bottomLeft, bottomRight, topRight };

	Vec2D xAxis = topRight - topLeft;
	Vec2D yAxis = bottomLeft - topLeft;

	const float invXAxisLengthSq = 1.0f / xAxis.Mag2();
	const float invYAxisLengthSq = 1.0f / yAxis.Mag2();

	FillPoly(points, [&](uint32_t px, uint32_t py) {
		
		Vec2D p = { static_cast<float>(px), static_cast<float>(py)};
		Vec2D d = p - topLeft;

		float u = invXAxisLengthSq * d.Dot(xAxis);
		float v = invYAxisLengthSq * d.Dot(yAxis);

		u = Clamp(u, 0.0f, 1.0f);
		v = Clamp(v, 0.0f, 1.0f);

		//float tx = roundf(u * static_cast<float>(sprite.width));
		//float ty = roundf(v * static_cast<float>(sprite.height));

		float tx = roundf(u * static_cast<float>(sprite.width - 1));
		float ty = roundf(v * static_cast<float>(sprite.height - 1));

		Color imageColor = pixels[GetIndex(image.GetWidth(), ty + sprite.yPos, tx + sprite.xPos)];

		Color newColor = { static_cast<uint8_t>(imageColor.GetRed() * rVal), static_cast<uint8_t>(imageColor.GetGreen() * gVal), static_cast<uint8_t>(imageColor.GetBlue() * bVal), static_cast<uint8_t>(imageColor.GetAlpha() * aVal) };

		return newColor;
		
	});




	//for(uint32_t r = 0; r < height; r++)
	//{
	//	for (uint32_t c = 0; c < width; c++) 
	//	{
	//		// this is full white
	//		Color imageColor = image.GetPixels()[GetIndex(image.GetWidth(), r + sprite.yPos, c + sprite.xPos)];

	//		Color newColor = {static_cast<uint8_t>( imageColor.GetRed() * rVal), static_cast<uint8_t>(imageColor.GetGreen() * gVal), static_cast<uint8_t>(imageColor.GetBlue() * bVal), static_cast<uint8_t>(imageColor.GetAlpha() * aVal )};

	//		Draw(c + pos.GetX(), r + pos.GetY(), newColor);
	//	}
	//}
}

void Screen::Draw(const SpriteSheet& ss, const std::string& spriteName, const Vec2D& pos, const Color& overlayColor, float rotation)
{
	Draw(ss.GetBMPImage(), ss.GetSprite(spriteName), pos, overlayColor, rotation);
}

void Screen::Draw(const BitmapFont& font, const std::string& textLine, const Vec2D& pos, const Color& overlayColor)
{
	uint32_t xPos = pos.GetX();

	const SpriteSheet& ss = font.GetSpriteSheet();

	for (char c : textLine) {
		if(c == ' ')
		{
			xPos += font.GetFontSpacingBetweenWords();
			continue;
		}

		Sprite sprite = ss.GetSprite(std::string("") + c);
		Draw(ss.GetBMPImage(), sprite, Vec2D(xPos, pos.GetY()), overlayColor);

		xPos += sprite.width;

		xPos += font.GetFontSpacingBetweenLetters();
	}

}

void Screen::ClearScreen() 
{
	assert(moptrWindow);
	if(moptrWindow)
	{
		if(mFast)
		{
			SDL_RenderClear(mRenderer);
		}
		else {

			SDL_FillRect(mnoptrWindowSurface, nullptr, mClearColor.GetPixelColor());
		}

	}
}

void Screen::FillPoly(const std::vector<Vec2D>& points, FillPolyFunc func)
{
	if (points.size() > 0)
	{
		float top = points[0].GetY();
		float bottom = points[0].GetY();
		float right = points[0].GetX();
		float left = points[0].GetX();

		for (size_t i = 1; i < points.size(); ++i)
		{
			if (points[i].GetY() < top)
			{
				top = points[i].GetY();
			}

			if (points[i].GetY() > bottom)
			{
				bottom = points[i].GetY();
			}

			if (points[i].GetX() < left)
			{
				left = points[i].GetX();
			}

			if (points[i].GetX() > right)
			{
				right = points[i].GetX();
			}
		}


		for (int pixelY = top; pixelY < bottom; ++pixelY)
		{
			std::vector<float> nodeXVec;

			size_t j = points.size() - 1;

			for (size_t i = 0; i < points.size(); ++i)
			{
				float pointiY = points[i].GetY();
				float pointjY = points[j].GetY();


				if ((pointiY <= (float)pixelY && pointjY > (float)pixelY) || (pointjY <= (float)pixelY && pointiY > (float)pixelY))
				{
					float denom = pointjY - pointiY;
					if (IsEqual(denom, 0))
					{
						continue;
					}

					float x = points[i].GetX() + (pixelY - pointiY) / (denom) * (points[j].GetX() - points[i].GetX());
					nodeXVec.push_back(x);
				}

				j = i;
			}

			std::sort(nodeXVec.begin(), nodeXVec.end(), std::less<>());

			for (size_t k = 0; k < nodeXVec.size(); k += 2)
			{
				if (nodeXVec[k] > right)
				{
					break;
				}

				if (nodeXVec[k + 1] > left)
				{
					if (nodeXVec[k] < left)
					{
						nodeXVec[k] = left;
					}
					if (nodeXVec[k + 1] > right)
					{
						nodeXVec[k + 1] = right;
					}

					//Line2D line = {Vec2D(nodeXVec[k], pixelY), Vec2D(nodeXVec[k+1], pixelY)};
					//Draw(line, color);
					for (int pixelX = nodeXVec[k]; pixelX < nodeXVec[k + 1]; ++pixelX)
					{
						Draw(pixelX, pixelY, func(pixelX, pixelY));
					}
				}
			}
		}
	}
}
