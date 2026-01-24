#pragma once

#include "Vec2D.h"
#include "Line2D.h"
#include "Shape.h"
#include <vector>

class Star2D : public Shape
{

public:

	Star2D();
	Star2D(float OuterRadius,float InnerRadius ,Vec2D Center, int Sections, bool randomStarSize = false);

	void RotateStar(float angle, Vec2D rotationPoint);

	virtual Vec2D GetCenterPoint() const override { return mCenter; };
	
	inline void SetOuterRadius(const float outerRadius) { mOuterRadius = outerRadius; }
	inline void SetInnerRadius(const float innerRadius) { mInnerRadius = innerRadius; }
	inline void SetSections(const int Sections) { mSections = Sections; }
	inline void SetRotationAngle(const float rot) { mRotationAngle = rot; }
	void SetCenterPoint(const Vec2D center);

	inline float GetRotationAngle() { return mRotationAngle; }
	inline float GetOuterRadius() { return mOuterRadius; }

	bool operator==(const Star2D& star) const;
	Star2D& operator=(const Star2D& other);

	float mOuterRadius, mInnerRadius;
	int mSections;
	Vec2D mCenter;
	float mRotationAngle;

	std::vector<Line2D> mStarLines;

	const float pi = 3.141592;



};
