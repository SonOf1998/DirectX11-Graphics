#include "pch.h"
#include "CollisionManager.h"

#include "BallObject.h"

bool CollisionManager::Intersects(BallObject* ball1, BallObject* ball2)
{
	float centerDiff = Length(ball1->GetPosition() - ball2->GetPosition());
	if (centerDiff + 0.0001 <= 2 * BALL_RADIUS)
	{
		return true;
	}

	return false;
}

struct Section
{
	// uncurved parts of the cushion //

/*
		
 top (yellow, brown, green)


		  cush 1
	   -----------
    h1			   h6
c	|				| c
u	|				| u
s	|				| s
h	|				| h
2	|				| 6
    h2			   h5
c	|				| c
u	|				| u
s	|				| s
h	|				| h
3	|				| 5
    h3             h4
	   -----------
	     cush 4

*/

	XMFLOAT2 start;
	XMFLOAT2 end;
	XMFLOAT2 normal;
	SECTION_INTERSECTION_OP op;

	bool Intersects(const XMFLOAT2& center) const
	{
		switch (op)
		{
		case SECTION_INTERSECTION_OP::LT_X:
			if (start.y <= center.y && center.y <= end.y)
			{
				if (center.x < start.x)
					return true;
			}
			break;
		case SECTION_INTERSECTION_OP::GT_X:
			if (start.y <= center.y && center.y <= end.y)
			{
				if (center.x > start.x)
					return true;
			}
			break;
		case SECTION_INTERSECTION_OP::LT_Z:
			if (start.x <= center.x && center.x <= end.x)
			{
				if (center.y < start.y)
					return true;
			}
			break;
		case SECTION_INTERSECTION_OP::GT_Z:
			if (start.x <= center.x && center.x <= end.x)
			{
				if (center.y > start.y)
					return true;
			}
			break;
		default:
			throw std::runtime_error("Section-intersection op switch should be exhaustive");
		}

		return false;
	}
};

struct Curvature
{
	XMFLOAT2 center;
	float radius;
};

Section sections[6] = {
	// BALL_RADIUS implicitly added to the measurements of the section edges //
	Section{XMFLOAT2(-2.4841f, -5.3436f), XMFLOAT2(2.4841f, -5.3436f), XMFLOAT2(0, 1), SECTION_INTERSECTION_OP::LT_Z},
	Section{XMFLOAT2(-2.6289f, -5.2f), XMFLOAT2(-2.6289f, -0.24776f), XMFLOAT2(1,0), SECTION_INTERSECTION_OP::LT_X},
	Section{XMFLOAT2(-2.6289f, 0.24776f), XMFLOAT2(-2.6289f, 5.2f), XMFLOAT2(1,0), SECTION_INTERSECTION_OP::LT_X},
	Section{XMFLOAT2(-2.4841f, 5.3436f), XMFLOAT2(2.4841f, 5.3436f), XMFLOAT2(0, -1), SECTION_INTERSECTION_OP::GT_Z},
	Section{XMFLOAT2(2.6289f, -5.2f), XMFLOAT2(2.6289f, -0.24776f), XMFLOAT2(-1,0), SECTION_INTERSECTION_OP::GT_X},
	Section{XMFLOAT2(2.6289f, 0.24776f), XMFLOAT2(2.6289f, 5.2f), XMFLOAT2(-1,0), SECTION_INTERSECTION_OP::GT_X},
};

Curvature curvature[12] = {
	Curvature {XMFLOAT2(-2.8825f, -0.24776f), 2.4f * BALL_RADIUS},
	Curvature {XMFLOAT2(-2.8825f, 0.24776f),  2.4f * BALL_RADIUS},
	Curvature {XMFLOAT2(2.8825f, -0.24776f),  2.4f * BALL_RADIUS},
	Curvature {XMFLOAT2(2.8825f, 0.24776f),   2.4f * BALL_RADIUS},
	Curvature {XMFLOAT2(2.5f, 5.616f),   2.6f * BALL_RADIUS},
	Curvature {XMFLOAT2(2.5f, -5.616f),  2.6f * BALL_RADIUS},
	Curvature {XMFLOAT2(-2.5f, 5.616f),  2.6f * BALL_RADIUS},
	Curvature {XMFLOAT2(-2.5f, -5.616f), 2.6f * BALL_RADIUS},
	Curvature {XMFLOAT2(2.9f, 5.232f),   2.6f * BALL_RADIUS},
	Curvature {XMFLOAT2(-2.9f, 5.232f),  2.6f * BALL_RADIUS},
	Curvature {XMFLOAT2(2.9f, -5.232f),  2.6f * BALL_RADIUS},
	Curvature {XMFLOAT2(-2.9f, -5.232f), 2.6f * BALL_RADIUS}
};


bool CollisionManager::IntersectsWall(BallObject* ball, XMFLOAT2& destNormal)
{
	XMVECTOR ballCenterV = ball->GetPosition();
	XMFLOAT2 pos2D = XMFLOAT2(XMVectorGetX(ballCenterV), XMVectorGetZ(ballCenterV));

	for (uint i = 0; i < 6; ++i)
	{
		if (sections[i].Intersects(pos2D))
		{
			destNormal = sections[i].normal;
			return true;	
		}
	}

	for (uint i = 0; i < 12; ++i)
	{
		XMFLOAT2 cc = curvature[i].center; // curve center
		float len = sqrtf((cc.x - pos2D.x) * (cc.x - pos2D.x) + (cc.y - pos2D.y) * (cc.y - pos2D.y));
		if (len < curvature[i].radius)
		{
			destNormal = XMFLOAT2((pos2D.x - cc.x) / len, (pos2D.y - cc.y) / len);
			return true;
		}
	}

	return false;
}

struct Hole
{
	XMVECTOR center;
	// radius is in positions.h //
};

Hole holes[6] = {
	Hole{XMVectorSet(-2.7757f, -5.5f, 0, 0)},
	Hole{XMVectorSet(-2.8565f, 0, 0, 0)},
	Hole{XMVectorSet(-2.7757f, 5.5f, 0, 0)},
	Hole{XMVectorSet(2.7757f, 5.5f, 0, 0)},
	Hole{XMVectorSet(2.8565f, 0, 0, 0)},
	Hole{XMVectorSet(2.7757f , -5.5f, 0, 0)},
};

bool CollisionManager::IntersectsHole(BallObject* ball)
{
	XMFLOAT2 ballPos2D = XMFLOAT2(XMVectorGetX(ball->GetPosition()), XMVectorGetZ(ball->GetPosition()));
	for (uint i = 0; i < 6; ++i)
	{
		if (XMVectorGetX(XMVector2Length(holes[i].center - XMLoadFloat2(&ballPos2D))) < HOLE_RADIUS)
		{
			return true;
		}
	}

	return false;
}
