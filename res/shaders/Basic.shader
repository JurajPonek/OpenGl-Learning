#shader vertex
#version 330 core  
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texturePos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{  
	gl_Position = projection * view * model * vec4(pos, 1.0); 
	TexCoords = texturePos;
	
	
};

#shader fragment
#version 330 core  
out vec4 Color;  
in vec2 TexCoords;

uniform sampler2D Texture1;
uniform sampler2D Texture2;

void main()  
{  
	Color = mix(texture(Texture1, TexCoords), texture(Texture2, vec2(1.0 - TexCoords.x, TexCoords.y)), 0.1f);
};
