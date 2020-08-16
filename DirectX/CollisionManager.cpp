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

c	|				| c
u	|				| u
s	|				| s
h	|				| h
2	|				| 6

c	|				| c
u	|				| u
s	|				| s
h	|				| h
3	|				| 5
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

Section sections[6] = {
	// BALL_RADIUS implicitly added to the measurements of the section edges //
	Section{XMFLOAT2(-2.4841f, -5.3436f), XMFLOAT2(2.4841f, -5.3436f), XMFLOAT2(0, 1), SECTION_INTERSECTION_OP::LT_Z},
	Section{XMFLOAT2(-2.6289f, -5.2f), XMFLOAT2(-2.6289f, -0.24776f), XMFLOAT2(1,0), SECTION_INTERSECTION_OP::LT_X},
	Section{XMFLOAT2(-2.6289f, 0.24776f), XMFLOAT2(-2.6289f, 5.2f), XMFLOAT2(1,0), SECTION_INTERSECTION_OP::LT_X},
	Section{XMFLOAT2(-2.4841f, 5.3436f), XMFLOAT2(2.4841f, 5.3436f), XMFLOAT2(0, -1), SECTION_INTERSECTION_OP::GT_Z},
	Section{XMFLOAT2(2.6289f, -5.2f), XMFLOAT2(2.6289f, -0.24776f), XMFLOAT2(-1,0), SECTION_INTERSECTION_OP::GT_X},
	Section{XMFLOAT2(2.6289f, 0.24776f), XMFLOAT2(2.6289f, 5.2f), XMFLOAT2(-1,0), SECTION_INTERSECTION_OP::GT_X},
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

	return false;
}
