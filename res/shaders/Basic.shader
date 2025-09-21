#shader vertex
#version 330 core  
layout (location = 0) in vec3 pos;  
layout (location = 1) in vec3 vertexColorAttrib;
uniform float horizontalOffset;
out vec4 vertexColor;
void main()
{  
	gl_Position = vec4(pos.x + horizontalOffset, pos.y, 0.0f, 1.0); 
	vertexColor = vec4(vertexColorAttrib.xyz, 1.0f);
};

#shader fragment
#version 330 core  
out vec4 Color;  
in vec4 vertexColor;
void main()  
{  
	Color = vertexColor;
};
