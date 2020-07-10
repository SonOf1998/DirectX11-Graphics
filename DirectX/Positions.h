#pragma once

// POS defines are actually translations, this is why their last coord is 0.

#define X_AXIS						XMVectorSet(1, 0, 0, 0)
#define Y_AXIS						XMVectorSet(0, 1, 0, 0)
#define Z_AXIS						XMVectorSet(0, 0, -1, 0)

#define SNOOKER_TABLE_POS_Y			0.65f
#define SNOOKER_TABLE_TRANSLATE		XMVectorSet(0, SNOOKER_TABLE_POS_Y, 0, 0)
#define SNOOKER_TABLE_SCALE			XMVectorSet(0.8f, 0.8f, 0.8f, 1)
#define SNOOKER_TABLE_ROT_AXIS		XMVectorSet(0, 1, 0, 0)
#define SNOOKER_TABLE_ROT_ANGLE		XM_PI / 2

#define SNOOKER_TABLE_FRICTION		0.3f

#define CUSHION_X_BORDER			2.626f
#define CUSHION_Z_BORDER			5.34f

// 1.0 -> no energy loss on hit
#define BALL_COLLISION_ENERGY_LOSS_FACTOR 0.9f

#define BALL_RADIUS					0.09f
#define BALL_SCALE					XMVectorSet(BALL_RADIUS, BALL_RADIUS, BALL_RADIUS, 1)
#define BALL_POS_Y					SNOOKER_TABLE_POS_Y + BALL_RADIUS * 1.8f
#define PINK_BALL_Z					-2.3f

#define WHITE_BALL_PREFERRED_POS	XMVectorSet(0.25f, BALL_POS_Y, 3.38f, 0)
#define YELLOW_BALL_POS				XMVectorSet(1, BALL_POS_Y, 3.38f, 0)
#define GREEN_BALL_POS				XMVectorSet(-1, BALL_POS_Y, 3.38f, 0)
#define BROWN_BALL_POS				XMVectorSet(0, BALL_POS_Y, 3.38f, 0)
#define BLUE_BALL_POS				XMVectorSet(0, BALL_POS_Y, 0, 0)
#define PINK_BALL_POS				XMVectorSet(0, BALL_POS_Y, PINK_BALL_Z, 0)
#define BLACK_BALL_POS				XMVectorSet(0, BALL_POS_Y, -4.3f, 0)

#define ARMCHAIR_SCALE				XMVectorSet(0.6f, 0.6f, 0.6f, 1)
#define ARMCHAIR_ROT_AXIS			XMVectorSet(0, 1, 0, 0)
#define ARMCHAIR_ROT_ANGLE			-XM_PI

#define ARMCHAIR_Y					-1.0f
#define ARMCHAIR_1_POS				XMVectorSet(-2, ARMCHAIR_Y, 10, 0)
#define ARMCHAIR_2_POS				XMVectorSet(2, ARMCHAIR_Y, 10, 0)

#define CHAIR_ELEVATION				0.5f
#define CHAIR_SCALE					XMVectorSet(0.4f, 0.4f, 0.4f, 1)