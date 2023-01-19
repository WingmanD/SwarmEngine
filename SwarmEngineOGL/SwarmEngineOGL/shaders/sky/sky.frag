#version 460 core
out vec4 FragColor;

in fragmentData {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} fragment;


struct Light {
    vec4 position;
    vec4 color;
    float intensity;
};

layout(std430, binding = 2) readonly buffer LightData {
    Light lights[];
};

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;
uniform float opacity;

layout (std140, binding = 0) uniform ViewData {
    mat4 view;
    mat4 projection;

    vec4 cameraPos;
    vec4 unlit;
};

uniform int diffuseMapPresent;
uniform sampler2D diffuseMap;

uniform int specularMapPresent;
uniform sampler2D specularMap;

void main()
{
    vec3 zenithColor = vec3(0.015, 0.39, 0.79);
    vec3 horizonColor = vec3(0.53, 0.81, 0.92);
    
    vec3 color = mix(horizonColor, zenithColor, abs(fragment.position.y) / 100.0);

    FragColor = vec4(color, opacity);
} 