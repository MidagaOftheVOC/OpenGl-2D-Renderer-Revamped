#shader vertex
#version 330 core
layout(location = 0) in vec2	b_Positions;
layout(location = 1) in vec2	b_TextureUVs;
layout(location = 2) in uint	b_Offsets;
layout(location = 3) in int		b_SpriteIndices;

//	standard uniforms
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

//	indexing for instances
uniform vec2 u_SpriteDimensions;
uniform int u_RowSpriteCount;


//	for line breaks
uniform int u_LineBreakCount;
uniform int u_LineBreakArray[64];

//uniform float u_LineMaximumLength;
uniform float u_LineLengths[64];


//	other text variables
uniform float u_CharacterHeight;
uniform float u_SpacingBetweenLines;

out vec2 v_TexCoord;

void main(){
	
	vec2 SpriteCoords = vec2(
		b_SpriteIndices % u_RowSpriteCount,
		b_SpriteIndices / u_RowSpriteCount
	);
	v_TexCoord = b_TextureUVs + SpriteCoords * u_SpriteDimensions;


	vec2 OffsetPosition = b_Positions;

	if( u_LineBreakCount > 0)
	{
		int PassedLineCount = 0;
		float TotalOffsetForCharacter = 0.f;
		for(int i = 0; i < u_LineBreakCount; i ++){
			if( gl_InstanceID >= u_LineBreakArray[i]){
				PassedLineCount ++;
				TotalOffsetForCharacter += u_LineLengths[i];
			}
		}

		OffsetPosition.x += float(b_Offsets) - TotalOffsetForCharacter;
		OffsetPosition.y += ( u_CharacterHeight + u_SpacingBetweenLines) * PassedLineCount;

	}
	else
		OffsetPosition += vec2(float(b_Offsets), 0.f);

	gl_Position = u_Projection * u_View * u_Model * vec4(OffsetPosition, 0.f, 1.f);
}

#shader fragment
#version 330 core


in vec2 v_TexCoord;

uniform sampler2D u_Texture;


out vec4 FragColour;

void main(){

	vec4 texel = texture(u_Texture, v_TexCoord);

	if(
		texel.x > 0.9 &&
		texel.y > 0.9 &&
		texel.z > 0.9
	){
		discard;
	}

	FragColour = vec4(1.f, 1.f, 1.f, 1.f);
}
