#shader vertex
#version 330 core  
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texturePos;
layout (location = 2) in vec3 anormals;

out vec3 normals;
out vec3 fragPosition;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 vp;
uniform mat3 normalMatrix;


void main()
{  
	gl_Position = vp * model * vec4(pos, 1.0); 
	fragPosition = vec3(model * vec4(pos, 1.0f));
	normals = normalMatrix * anormals;
	TexCoords = texturePos;
};

#shader fragment
#version 330 core  
#define NUMBER_OF_POINT_LIGHTS 4
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess; 
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
    bool enabled;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct SpotLight
{
    vec3 position;
    vec3 direction;
    float innerCutOff;
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
};
struct LightEnabled
{
    bool directional;
    bool spot;
};

out vec4 Color;  

in vec3 normals;
in vec3 fragPosition;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;           
uniform DirectionalLight directionalLight;  
uniform LightEnabled enabled;
uniform SpotLight spotLight;

uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];  


vec3 CalculateDirectionalLight(DirectionalLight directionalLight, vec3 normal, vec3 viewDir, vec3 diffuseMap, vec3 specularMap)
{
    vec3 lightDirD = normalize(-directionalLight.direction);
    float diffD = max(dot(normal, lightDirD), 0.0);
    vec3 reflectDirD = reflect(-lightDirD, normal);
    float specD = pow(max(dot(viewDir, reflectDirD), 0.0), material.shininess);
    vec3 ambientD  = directionalLight.ambient  * diffuseMap;
    vec3 diffuseD  = directionalLight.diffuse  * diffD * diffuseMap;
    vec3 specularD = directionalLight.specular * specD * specularMap;
    vec3 totalDirectionalLight = ambientD + diffuseD + specularD;
    return totalDirectionalLight;
}

vec3 CalculatePointLight(PointLight Pointlight, vec3 normal, vec3 viewDir, vec3 diffuseMap, vec3 specularMap)
{
    vec3 lightDirP = normalize(Pointlight.position - fragPosition);
    float pointLightDistance = length(Pointlight.position - fragPosition);
    float attenuation = 1.0 / (Pointlight.attenuation.x + Pointlight.attenuation.y * pointLightDistance + Pointlight.attenuation.z * pointLightDistance * pointLightDistance);
    float diffP = max(dot(normal, lightDirP), 0.0);
    vec3 reflectDirP = reflect(-lightDirP, normal);
    float specP = pow(max(dot(viewDir, reflectDirP), 0.0), material.shininess);
    vec3 ambientP  = Pointlight.ambient  * diffuseMap * attenuation;
    vec3 diffuseP  = Pointlight.diffuse  * diffP * diffuseMap * attenuation;
    vec3 specularP = Pointlight.specular * specP * specularMap * attenuation;
    vec3 totalPointLight = ambientP + diffuseP + specularP;
    return totalPointLight;

}

vec3 CalculateSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDir, vec3 diffuseMap, vec3 specularMap)
{
    vec3 lightDirS = normalize(spotLight.position - fragPosition);
    float theta = dot(lightDirS, normalize(-spotLight.direction));
    float epsilon = spotLight.innerCutOff - spotLight.outerCutOff;
    float Intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0f, 1.0f);
    float pointLightDistanceS = length(spotLight.position - fragPosition);
    float attenuationS = 1.0 / (spotLight.attenuation.x + spotLight.attenuation.y * pointLightDistanceS + spotLight.attenuation.z * pointLightDistanceS * pointLightDistanceS);
    float diffS = max(dot(normal, lightDirS), 0.0);
    vec3 reflectDirS = reflect(-lightDirS, normal);
    float specS = pow(max(dot(viewDir, reflectDirS), 0.0), material.shininess);
    vec3 ambientS  = spotLight.ambient  * diffuseMap * attenuationS * Intensity;
    vec3 diffuseS  = spotLight.diffuse  * diffS * diffuseMap * attenuationS * Intensity;
    vec3 specularS = spotLight.specular * specS * specularMap * attenuationS * Intensity;
    vec3 totalSpotLight = ambientS + diffuseS + specularS;
    return totalSpotLight;

}



void main()  
{  
    
    vec3 diffuseMap = vec3(texture(material.diffuse, TexCoords));
    vec3 specularMap = vec3(texture(material.specular, TexCoords));
    vec3 normal = normalize(normals);
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 lightning = vec3(0.0f);
    
    if (enabled.directional)
    {
        lightning += CalculateDirectionalLight(directionalLight, normal, viewDir, diffuseMap, specularMap);
    }
    for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
    {
        if (pointLights[i].enabled)
        {
            lightning += CalculatePointLight(pointLights[i], normal, viewDir, diffuseMap, specularMap);
        }   
    }
    
    if (enabled.spot)
    {
      
        lightning += CalculateSpotLight(spotLight, normal, viewDir, diffuseMap, specularMap);
        
    }
    
    
    
    Color = vec4(lightning, 1.0);
}
