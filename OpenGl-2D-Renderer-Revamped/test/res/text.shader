#shader vertex
#version 330 core
layout(location = 0) in vec2	b_Positions;
layout(location = 1) in vec2	b_TextureUVs;
layout(location = 2) in uint	b_Offsets;
layout(location = 3) in int		b_SpriteIndices;


uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform vec2 u_SpriteDimensions;
uniform int u_RowSpriteCount;

out vec2 v_TexCoord;

void main(){
	
	vec2 SpriteCoords = vec2(
		b_SpriteIndices % u_RowSpriteCount,
		b_SpriteIndices / u_RowSpriteCount
	);

	v_TexCoord = b_TextureUVs + SpriteCoords * u_SpriteDimensions;



	vec2 OffsetPosition = b_Positions + vec2(float(b_Offsets), 0.f);

	gl_Position = u_Projection * u_View * u_Model * vec4(OffsetPosition, 0.f, 1.f);
}

#shader fragment
#version 330 core


in vec2 v_TexCoord;

uniform sampler2D u_Texture;


out vec4 FragColour;

void main(){
	FragColour = mix(texture(u_Texture, v_TexCoord), vec4(1.f, 0.f, 0.f, 1.f), 0.2f);
}
