#shader vertex
#version 330 core  
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texturePos;


uniform mat4 model;
uniform mat4 vp;
out vec2 textureCoords;

void main()
{  
	gl_Position = vp * model * vec4(pos, 1.0); 
	textureCoords = texturePos;
	vec3 dummy = aNormal; 
};

#shader fragment
#version 330 core  
out vec4 Color;  
uniform sampler2D tex;

in vec2 textureCoords;

void main()  
{  
	Color = texture(tex, textureCoords);
};