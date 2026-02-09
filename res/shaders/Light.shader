#shader vertex
#version 330 core  
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texturePos;


uniform mat4 model;
uniform mat4 vp;

void main()
{  
	gl_Position = vp * model * vec4(pos, 1.0); 
	
	
	
};

#shader fragment
#version 330 core  
out vec4 Color;  
uniform vec3 u_lightColor;




void main()  
{  
	Color = vec4(u_lightColor, 1.0f);
};