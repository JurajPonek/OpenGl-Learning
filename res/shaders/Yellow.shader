#shader vertex
#version 330 core
layout (location = 0) in vec2 VertexPos;
out vec4 vertexColor;
void main()
{
	gl_Position = vec4(VertexPos, 0.0f, 1.0f);
	vertexColor = vec4(VertexPos, 0.0f, 1.0f);
};

#shader fragment
#version 330 core
out vec4 FragColor;
in vec4 vertexColor;
void main()
{
	FragColor = vertexColor;
};