#version 330 core

in vec2 textureCoord;

uniform sampler2D particlesTex;
uniform sampler2D snowMap;
uniform sampler2D heightMap;

uniform int calendar;

layout(location = 0) out vec3 FragColor;

uniform vec2 resolution = vec2(256.0,256.0);

void main(){

    vec2 uv = gl_FragCoord.xy/resolution.xy;


    vec3 particule = texture2D(particlesTex,textureCoord).rgb;
    float height = texture2D(heightMap,vec2(particule.x,particule.z)).r;
    float snow =  texture2D(snowMap,uv).r;

    if(particule.x+0.005 >= uv.x && particule.x-0.005 <= uv.x && particule.z+0.005 >= uv.y && particule.z-0.005 <= uv.y && particule.y <= height+(0.1/4.0) && calendar<170)
        snow += 0.8;
    else
        snow -= 0.002;
    FragColor = vec3(snow);

}
