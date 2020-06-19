#pragma once


// ... MODELS ...

#define SNOOKER_TABLE_MODEL					"Models/table.fbx"
#define BALL_MODEL							"Models/ball.obj"
#define UNIT_TETRAHEDRON_MODEL				"Models/tetrahedron.obj"

// ... TEXTURES ...

#define TEXTURE_ROOTDIR						L"Textures/"

#define TABLE_FRAME_TEXTURE					TEXTURE_ROOTDIR "solid_wood.png"
#define TABLE_CLOTH_TEXTURE					TEXTURE_ROOTDIR "green_fabric.png"
#define TABLE_XINGPAISTAR_TEXTURE			TEXTURE_ROOTDIR "xingpaistar.png"
#define TABLE_CUSHION_TEXTURE				TEXTURE_ROOTDIR "side_wall_fabric.png"
#define	TABLE_POCKET_TEXTURE				TEXTURE_ROOTDIR "pocket_dimmer.png"											  
														    
#define WHITE_BALL_TEXTURE					TEXTURE_ROOTDIR "whiteball.png"
#define RED_BALL_TEXTURE					TEXTURE_ROOTDIR "redball.png"		
#define YELLOW_BALL_TEXTURE					TEXTURE_ROOTDIR "yellowball.png"	
#define GREEN_BALL_TEXTURE					TEXTURE_ROOTDIR "greenball.png"	
#define BROWN_BALL_TEXTURE					TEXTURE_ROOTDIR "brownball.png"	
#define BLUE_BALL_TEXTURE					TEXTURE_ROOTDIR "blueball.png"		
#define PINK_BALL_TEXTURE					TEXTURE_ROOTDIR "pinkball.png"		
#define BLACK_BALL_TEXTURE					TEXTURE_ROOTDIR "blackball.png"	


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








