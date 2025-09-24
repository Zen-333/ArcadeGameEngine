#pragma once

#include "Vec2D.h"
#include "Line2D.h"
#include "Shape.h"
#include <vector>

class Star2D : public Shape
{

public:

	Star2D();
	Star2D(float OuterRadius,float InnerRadius ,Vec2D Center, int Sections);

	void RotateStar(float angle, Vec2D rotationPoint);

	virtual Vec2D GetCenterPoint() const override { return mCenter; };

	float mOuterRadius, mInnerRadius;
	int mSections;
	Vec2D mCenter;

	std::vector<Line2D> mStarLines;

	const float pi = 3.141592;



};
