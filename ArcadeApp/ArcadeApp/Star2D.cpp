
#include "Star2D.h"
#include "Vec2D.h"

Star2D::Star2D(): mOuterRadius(2.0f),mInnerRadius(1.0f), mCenter(0, 0), mSections(0)
{

}

Star2D::Star2D(float OuterRadius,float InnerRadius ,Vec2D Center, int Sections): mOuterRadius(OuterRadius),mInnerRadius(InnerRadius), mCenter(Center), mSections(Sections)
{
	if (InnerRadius >= OuterRadius) OuterRadius = InnerRadius * InnerRadius;

	int doubleSections = mSections * 2;
	float angleStep = ((pi * 2.0f) / doubleSections);


	//std::vector<Vec2D> vertices;

	for(int i = 0; i < doubleSections; i++)
	{
		float angle = i * angleStep;
		float r = (i % 2 == 0) ? mOuterRadius : mInnerRadius;

		float x = mCenter.GetX() + r * cosf(angle);
		float y = mCenter.GetY() + r * sinf(angle);

		mPoints.push_back(Vec2D(x, y)); //
	}

	for(int i = 0; i < mPoints.size(); i++) //
	{
		Vec2D p0 = mPoints[i]; //
		Vec2D p1 = mPoints[(i + 1) % mPoints.size()]; //
		mStarLines.push_back(Line2D(p0, p1));
	}

}

void Star2D::RotateStar(float angle, Vec2D rotationPoint)
{
	for(int i = 0; i < mStarLines.size(); i++)
	{
		mStarLines[i].Rotate(angle, rotationPoint);
	
	}
}

