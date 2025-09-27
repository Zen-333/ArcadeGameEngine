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


Screen::Screen() : mWidth(0), mHeight(0), moptrWindow(nullptr), mnoptrWindowSurface(nullptr)
{

}

Screen::~Screen() 
{
	if(moptrWindow)
	{
		SDL_DestroyWindow(moptrWindow);
		moptrWindow = nullptr;
	}

	SDL_Quit();
}

SDL_Window* Screen::Init(uint32_t w, uint32_t h, uint32_t mag)
{
	if (SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << "Error SDL_Init Failed: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	mWidth = w;
	mHeight = h;

	moptrWindow = SDL_CreateWindow("Arcade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth * mag, mHeight * mag, 0);

	if (moptrWindow)
	{
		mnoptrWindowSurface = SDL_GetWindowSurface(moptrWindow); // front buffer
		SDL_PixelFormat* pixelFormat = mnoptrWindowSurface->format;

		Color::InitColorFormat(pixelFormat);

		mClearColor = Color::Black();

		mBackBuffer.Init(pixelFormat->format, mWidth, mHeight);
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

		SDL_BlitScaled(mBackBuffer.GetSurface(), nullptr, mnoptrWindowSurface, nullptr);

		//Why “scaled” ?

			//If your window is bigger than your back buffer(because of mag), SDL will stretch it automatically.

			//Example : back buffer = 224x288, window = 672x864 ? each pixel is 3×3 on the screen.

		SDL_UpdateWindowSurface(moptrWindow);

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
		FillPoly(star.GetPoints(), fillColor);
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
		FillPoly(triangle.GetPoints(), fillColor);
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
		FillPoly(rectangle.GetPoints(), fillColor);
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
		FillPoly(circlePoints, fillColor);
	}

	for(const Line2D& line : lines)
	{
		Draw(line, color);
	}

}

void Screen::ClearScreen() 
{
	assert(moptrWindow);
	if(moptrWindow)
	{
		SDL_FillRect(mnoptrWindowSurface, nullptr, mClearColor.GetPixelColor());

	}
}

void Screen::FillPoly(const std::vector<Vec2D>& points, const Color& color)
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
						Draw(pixelX, pixelY, color);
					}
				}
			}
		}




	}
}
