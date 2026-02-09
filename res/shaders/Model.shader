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
in vec2 textureCoords;
struct Material {
	 sampler2D texture_diffuse1;
	 sampler2D texture_diffuse2;
	 sampler2D texture_diffuse3;
	 sampler2D texture_specular1;
	 sampler2D texture_specular2;
};
uniform Material material;
float near = 0.1f;
float far = 100.0f;
float linearizeDepth(float depth)
{
	float ndc = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - ndc * (far - near));
}

float fogDensity = 0.1; 



void main()  
{  
    
    float dist = linearizeDepth(gl_FragCoord.z);
    
    
  
    float fogFactor = exp(-pow(dist * fogDensity, 2.0));
    fogFactor = clamp(fogFactor, 0.0, 1.0); 

    
    vec3 fogColor = vec3(0.902, 0.902, 0.980);
    vec4 texColor = texture(material.texture_diffuse1, textureCoords);
    vec3 objColor = texColor.rgb;

   
    vec3 finalColor = mix(fogColor, objColor, fogFactor);
    
    Color = vec4(finalColor, texColor.a); 
}