#shader vertex
#version 330 core
layout(location = 0) in vec2 b_Positions;
layout(location = 1) in vec2 b_TextureUVs;


uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec2 v_TexCoord;

void main(){
	gl_Position = u_Projection * u_View * u_Model * vec4(b_Positions, 0.f, 1.f);
	v_TexCoord = b_TextureUVs;
}

#shader fragment
#version 330 core

in vec2 v_TexCoord;
out vec4 FragColour;


uniform sampler2D u_Texture;


void main(){
	FragColour = texture(u_Texture, v_TexCoord);
	//FragColour = vec4(1.f, 0.f, 0.f, 1.f);
}



