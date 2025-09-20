#pragma once

#include "Vec2D.h"
#include "Line2D.h"
#include <vector>

class Star2D
{

public:

	Star2D();
	Star2D(float OuterRadius,float InnerRadius ,Vec2D Center, int Sections);

	void RotateStar(float angle, Vec2D rotationPoint);

	float mOuterRadius, mInnerRadius;
	int mSections;
	Vec2D mCenter;

	std::vector<Line2D> mStarLines;

	const float pi = 3.141592;



};
