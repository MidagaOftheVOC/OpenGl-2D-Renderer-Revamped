#shader vertex
#version 430 core

layout(location = 0) in vec2 b_VertexBuffer;
layout(location = 1) in vec2 b_TexBuffer;
layout(location = 2) in uint b_SpriteInformationBuffer;
layout(location = 3) in float b_Rotations;
layout(location = 4) in vec2 b_PositionsRelativeToModel;
layout(location = 5) in vec2 b_QuadDimensions;

layout(std140, binding = 0) uniform ubo_UVRegions {
    vec4 u_UVRegions[512];
};

layout(std140, binding = 1) uniform ubo_SheetOffsets {
    int u_SheetOffsets[32];
};

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;


out vec2 v_TextureVertex;
flat out uint v_SheetIndex;



void main(){

    // Texture region mapping
    uint SheetIndex  = uint( b_SpriteInformationBuffer >> 11 );
    uint SpriteIndex = uint( b_SpriteInformationBuffer & 0x07FFu );
    v_SheetIndex = SheetIndex;

    
    int TotalOffset = 0;
	for(int i = 0; i < SheetIndex; i ++){
		TotalOffset += u_SheetOffsets[i];
	}

    vec4 UVRegion = u_UVRegions[TotalOffset + SpriteIndex];

    vec2 MinUV = UVRegion.xy;
    vec2 MaxUV = UVRegion.zw;

    v_TextureVertex = mix(MinUV, MaxUV, b_TexBuffer);

    //  Coord calculation
    vec2 NormalisedVertexBuffer = b_VertexBuffer / 100.f;
    vec2 LocalVertex = b_QuadDimensions * NormalisedVertexBuffer;
    vec2 PointOfRotation = LocalVertex - b_QuadDimensions / 2;

    float sine = sin(b_Rotations);
    float cosine = cos(b_Rotations);

    vec2 RotatedPosition = vec2(
		PointOfRotation.x * cosine - PointOfRotation.y * sine,
		PointOfRotation.x * sine + PointOfRotation.y * cosine
	);

    vec2 WorldPosition = RotatedPosition + b_PositionsRelativeToModel;

	gl_Position = u_Projection * u_View * u_Model * vec4(WorldPosition, 0.f, 1.f);
}



#shader fragment
#version 430 core

in vec2 v_TextureVertex;
flat in uint v_SheetIndex;


uniform sampler2D u_Textures[32];


out vec4 FragColour;

void main(){
	FragColour = texture(u_Textures[v_SheetIndex], v_TextureVertex);
}


