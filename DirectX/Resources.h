#pragma once


// ... MODELS ...

#define MODEL_ROOTDIR						"Models/"

#define SNOOKER_TABLE_MODEL					MODEL_ROOTDIR "table.fbx"
#define BALL_MODEL							MODEL_ROOTDIR "ball.obj"
#define UNIT_TETRAHEDRON_MODEL				MODEL_ROOTDIR "tetrahedron.obj"
#define LOW_QUALITY_SPHERE_MODEL			MODEL_ROOTDIR "low_quality_sphere.fbx"
#define ARMCHAIR_MODEL						MODEL_ROOTDIR "armchair.fbx"
#define CHAIR_MODEL							MODEL_ROOTDIR "chair.fbx"
#define CUE_MODEL							MODEL_ROOTDIR "cue.fbx"

// ... TEXTURES ...

#define TEXTURE_ROOTDIR						L"Textures/"

#define TABLE_FRAME_TEXTURE					TEXTURE_ROOTDIR "solid_wood.jpg"
#define TABLE_CLOTH_TEXTURE					TEXTURE_ROOTDIR "green_fabric.jpg"
#define TABLE_XINGPAISTAR_TEXTURE			TEXTURE_ROOTDIR "xingpaistar.jpg"
#define TABLE_CUSHION_TEXTURE				TEXTURE_ROOTDIR "side_wall_fabric.jpg"
#define	TABLE_POCKET_TEXTURE				TEXTURE_ROOTDIR "pocket_dimmer.jpg"											  
														    
#define WHITE_BALL_TEXTURE					TEXTURE_ROOTDIR "whiteball.png"
#define RED_BALL_TEXTURE					TEXTURE_ROOTDIR "redball.png"		
#define YELLOW_BALL_TEXTURE					TEXTURE_ROOTDIR "yellowball.png"	
#define GREEN_BALL_TEXTURE					TEXTURE_ROOTDIR "greenball.png"	
#define BROWN_BALL_TEXTURE					TEXTURE_ROOTDIR "brownball.png"	
#define BLUE_BALL_TEXTURE					TEXTURE_ROOTDIR "blueball.png"		
#define PINK_BALL_TEXTURE					TEXTURE_ROOTDIR "pinkball.png"		
#define BLACK_BALL_TEXTURE					TEXTURE_ROOTDIR "blackball.png"	

#define ARMCHAIR_PILLOW_TEXTURE				TEXTURE_ROOTDIR "pillow.jpg"
#define ARMCHAIR_BUMP_TEXTURE				TEXTURE_ROOTDIR "bump.png"
#define ARMCHAIR_FRAME_TEXTURE				TEXTURE_ROOTDIR "wood.jpg"

#define CHAIR_FABRIC_TEXTURE				TEXTURE_ROOTDIR "dark_blue_fabric.jpg"
#define CHAIR_FRAME_TEXTURE					TEXTURE_ROOTDIR "light_wood.jpg"

#define CUE_LEATHER_TEXTURE					TEXTURE_ROOTDIR "dark_blue_fabric.jpg"
#define CUE_FLANGE_TEXTURE					TEXTURE_ROOTDIR "yellowball.png"
#define CUE_LIGHTWOOD_TEXTURE				TEXTURE_ROOTDIR "light_wood.jpg"
#define CUE_DARKWOOD_TEXTURE				TEXTURE_ROOTDIR "solid_wood.jpg"

// ... SOUNDS ...

#define SOUNDS_ROOTDIR						L"WaveFiles/"

#define TEST_SOUND							SOUNDS_ROOTDIR "ball.wav"
#define WALL_HIT_SOUND						SOUNDS_ROOTDIR "wall_hit.wav"
#define HOLE_SOUND							SOUNDS_ROOTDIR "hole.wav"
#define BALL_HIT_SOUND						SOUNDS_ROOTDIR "ball_hit.wav"
#define LIGHT_TOUCHING_SOUND				SOUNDS_ROOTDIR "light_touching.wav"


// ... SHADERS ....

#define SHADER_ROOTDIR						L"Shaders/"
#define BINARY_EXT							".cso"


#define VS									"vs" BINARY_EXT
#define HS									"hs" BINARY_EXT
#define DS									"ds" BINARY_EXT
#define GS									"gs" BINARY_EXT
#define PS									"ps" BINARY_EXT
#define CS									"cs" BINARY_EXT

#define BALL_HWTESS_LOD						"Ball-HWTessellationWithLoD/bin/"
#define BALL_HWTESS_LOD_VS					SHADER_ROOTDIR BALL_HWTESS_LOD VS
#define BALL_HWTESS_LOD_HS					SHADER_ROOTDIR BALL_HWTESS_LOD HS
#define BALL_HWTESS_LOD_DS					SHADER_ROOTDIR BALL_HWTESS_LOD DS
#define BALL_HWTESS_LOD_PS					SHADER_ROOTDIR BALL_HWTESS_LOD PS

#define SHADOWMAP_DIRECTIONAL				"ShadowMap-Directional/bin/"
#define SHADOWMAP_DIRECTIONAL_VS			SHADER_ROOTDIR SHADOWMAP_DIRECTIONAL VS
#define SHADOWMAP_DIRECTIONAL_PS			SHADER_ROOTDIR SHADOWMAP_DIRECTIONAL PS

#define BEZIER_QUAD							"BezierQuad/bin/"
#define BEZIER_QUAD_VS						SHADER_ROOTDIR BEZIER_QUAD VS
#define BEZIER_QUAD_HS						SHADER_ROOTDIR BEZIER_QUAD HS
#define BEZIER_QUAD_DS						SHADER_ROOTDIR BEZIER_QUAD DS
#define BEZIER_QUAD_PS						SHADER_ROOTDIR BEZIER_QUAD PS

// ... CLEAR COLORS ...

namespace COLOR
{
	constexpr float SM_WHITE[4] = { 100000, 100000, 100000, 1 };
	constexpr float WHITE[4] = { 1, 1, 1, 1 };
	constexpr float BLACK[4] = { 0, 0, 0, 1 };
	constexpr float GRAY[4] = { 0.3f, 0.3f, 0.3f, 1 };
}







