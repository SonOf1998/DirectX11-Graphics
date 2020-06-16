#pragma once

#define SNOOKER_TABLE_POS_Y			0.65f
#define SNOOKER_TABLE_TRANSLATE		XMVectorSet(0, SNOOKER_TABLE_POS_Y, 0, 0)
#define SNOOKER_TABLE_SCALE			XMVectorSet(0.8f, 0.8f, 0.8f, 1)
#define SNOOKER_TABLE_ROT_AXIS		XMVectorSet(0, 1, 0, 0)
#define SNOOKER_TABLE_ROT_ANGLE		XM_PI / 2

#define BALL_RADIUS					0.1f
#define BALL_SCALE					XMVectorSet(BALL_RADIUS, BALL_RADIUS, BALL_RADIUS, 1)
#define BALL_POS_Y					SNOOKER_TABLE_POS_Y + 0.16f
#define PINK_BALL_Z					-2.3f

#define WHITE_BALL_PREFERRED_POS	XMVectorSet(1, BALL_POS_Y, 3, 1)
#define YELLOW_BALL_POS				XMVectorSet(1.37f, BALL_POS_Y, 2.7f, 1)
#define GREEN_BALL_POS				XMVectorSet(-1.37f, BALL_POS_Y, 2.7f, 1)
#define BROWN_BALL_POS				XMVectorSet(0, BALL_POS_Y, 2.7f, 1)
#define BLUE_BALL_POS				XMVectorSet(0, BALL_POS_Y, 0, 1)
#define PINK_BALL_POS				XMVectorSet(0, BALL_POS_Y, PINK_BALL_Z, 1)
#define BLACK_BALL_POS				XMVectorSet(0, BALL_POS_Y, -4.3f, 1)
